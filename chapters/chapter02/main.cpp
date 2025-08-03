#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <filesystem>
#include <string>

struct MyAppState
{
	SDL_Window* window = nullptr;
	SDL_GPUDevice* device = nullptr;
	SDL_GPUGraphicsPipeline* pipeline = nullptr;
};

// Adapted from: https://github.com/TheSpydog/SDL_gpu_examples/blob/main/Examples/Common.c
SDL_GPUShader* LoadShader(
	SDL_GPUDevice* device,
	const std::string& shaderFilename,
	const Uint32 samplerCount,
	const Uint32 uniformBufferCount,
	const Uint32 storageBufferCount,
	const Uint32 storageTextureCount
)
{
	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (shaderFilename.contains(".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (shaderFilename.contains(".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDL_Log("Couldn't deduce shader stage from file name: %s", shaderFilename.c_str());
		return nullptr;
	}

	const SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);

	std::filesystem::path fullPath = std::filesystem::path(SDL_GetBasePath()) / "shaders";
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
	{
		fullPath /= shaderFilename + ".spv";
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
	{
		fullPath /= shaderFilename + ".msl";
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main0";
	}
	else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
	{
		fullPath /= shaderFilename + ".dxil";
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	}
	else
	{
		SDL_Log("Couldn't find a supported shader format for backend %s!", SDL_GetGPUDeviceDriver(device));
		return nullptr;
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath.string().c_str(), &codeSize);
	if (code == nullptr)
	{
		SDL_Log("Couldn't load shader file from disk!\n\t%s", SDL_GetError());
		return nullptr;
	}

	const SDL_GPUShaderCreateInfo shaderInfo = SDL_GPUShaderCreateInfo{
		.code_size = codeSize,
		.code = static_cast<Uint8*>(code),
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
		.num_samplers = samplerCount,
		.num_storage_textures = storageTextureCount,
		.num_storage_buffers = storageBufferCount,
		.num_uniform_buffers = uniformBufferCount,
	};
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == nullptr)
	{
		SDL_Log("Couldn't create shader from file %s: %s", fullPath.c_str(), SDL_GetError());
		SDL_free(code);
		return nullptr;
	}

	SDL_free(code);
	return shader;
}

// This function runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	MyAppState* myAppState = new MyAppState();
	*appstate = myAppState;

	myAppState->window = SDL_CreateWindow("Hello, SDL GPU!", 1280, 720, 0);
	if (myAppState->window == nullptr)
	{
		SDL_Log("Couldn't create window: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_GPUShaderFormat formatFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
	myAppState->device = SDL_CreateGPUDevice(formatFlags, true, nullptr);
	if (myAppState->device == nullptr)
	{
		SDL_Log("Couldn't create GPU device: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_ClaimWindowForGPUDevice(myAppState->device, myAppState->window))
	{
		SDL_Log("Couldn't claim window for GPU device: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Shader files from: https://github.com/TheSpydog/SDL_gpu_examples/tree/main/Content/Shaders/
	SDL_GPUShader* vertexShader = LoadShader(myAppState->device, "RawTriangle.vert", 0, 0, 0, 0);
	if (vertexShader == nullptr)
	{
		SDL_Log("Couldn't create vertex shader!");
		return SDL_APP_FAILURE;
	}

	SDL_GPUShader* fragmentShader = LoadShader(myAppState->device, "SolidColor.frag", 0, 0, 0, 0);
	if (fragmentShader == nullptr)
	{
		SDL_Log("Couldn't create fragment shader!");
		return SDL_APP_FAILURE;
	}

	const SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo{
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = SDL_GPURasterizerState{
			.fill_mode = SDL_GPU_FILLMODE_FILL,
		},
		.target_info = SDL_GPUGraphicsPipelineTargetInfo{
			.color_target_descriptions = (SDL_GPUColorTargetDescription[]){
				SDL_GPUColorTargetDescription{
					.format = SDL_GetGPUSwapchainTextureFormat(myAppState->device, myAppState->window)
				}
			},
			.num_color_targets = 1,
		},
	};

	myAppState->pipeline = SDL_CreateGPUGraphicsPipeline(myAppState->device, &pipelineCreateInfo);
	if (myAppState->pipeline == nullptr)
	{
		SDL_Log("Couldn't create graphics pipeline! %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_ReleaseGPUShader(myAppState->device, vertexShader);
	SDL_ReleaseGPUShader(myAppState->device, fragmentShader);

	// Continue running the application
	return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_EVENT_QUIT:
		// Quit the application with a success state
		return SDL_APP_SUCCESS;
	default:
		// Continue running the application
		return SDL_APP_CONTINUE;
	}
}

// This function runs once per frame, and is the heart of the application
SDL_AppResult SDL_AppIterate(void* appstate)
{
	MyAppState* myAppState = static_cast<MyAppState*>(appstate);

	SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(myAppState->device);
	if (commandBuffer == nullptr)
	{
		SDL_Log("Couldn't acquire GPU command buffer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_GPUTexture* swapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, myAppState->window, &swapchainTexture, nullptr, nullptr))
	{
		SDL_Log("Couldn't acquire swapchain texture: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (swapchainTexture != nullptr)
	{
		SDL_GPUColorTargetInfo colorTargetInfo = {
			.texture = swapchainTexture,
			.clear_color = SDL_FColor{0.4f, 0.6f, 0.9f, 1.0f},
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_STORE,
		};

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, nullptr);

		SDL_BindGPUGraphicsPipeline(renderPass, myAppState->pipeline);
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(commandBuffer);

	// Continue running the application
	return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	switch (result)
	{
	case SDL_APP_SUCCESS:
		SDL_Log("Application quit successfully");
		break;
	case SDL_APP_FAILURE:
		SDL_Log("Application quit with an error");
		break;
	default:
		SDL_Log("Application quit with an unexpected result");
		break;
	}
	MyAppState* myAppState = static_cast<MyAppState*>(appstate);
	SDL_ReleaseGPUGraphicsPipeline(myAppState->device, myAppState->pipeline);
	SDL_ReleaseWindowFromGPUDevice(myAppState->device, myAppState->window);
	SDL_DestroyWindow(myAppState->window);
	SDL_DestroyGPUDevice(myAppState->device);
	delete myAppState;
}
