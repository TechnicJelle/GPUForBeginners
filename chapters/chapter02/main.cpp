#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <array>
#include <filesystem>
#include <span>
#include <string>

struct Vertex
{
	float x, y, z;
};

struct MyAppState
{
	SDL_Window* window = nullptr;
	SDL_GPUDevice* device = nullptr;

	SDL_GPUGraphicsPipeline* pipeline = nullptr;
	SDL_GPUBuffer* vertexBuffer = nullptr;
	Uint32 numVertices = 0;
};

// Adapted from: https://github.com/TheSpydog/SDL_gpu_examples/blob/main/Examples/Common.c
SDL_GPUShader* LoadShader(SDL_GPUDevice* device, const std::string& shaderFilename)
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

	std::filesystem::path fullPath = std::filesystem::path(SDL_GetBasePath()) / "shaders";
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char* entrypoint;

	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
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

	size_t fileSize;
	void* code = SDL_LoadFile(fullPath.string().c_str(), &fileSize);
	if (code == nullptr)
	{
		SDL_Log("Couldn't load shader file from disk!\n\t%s", SDL_GetError());
		return nullptr;
	}

	SDL_GPUShaderCreateInfo shaderInfo = SDL_GPUShaderCreateInfo{
		.code_size = fileSize,
		.code = static_cast<Uint8*>(code),
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
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

/// Returns true on success, false on failure
bool CreatePipeline(MyAppState* myAppState)
{
	SDL_GPUShader* vertexShader = LoadShader(myAppState->device, "OnlyPosition.vert");
	if (vertexShader == nullptr)
	{
		SDL_Log("Couldn't create vertex shader!");
		return false;
	}

	SDL_GPUShader* fragmentShader = LoadShader(myAppState->device, "SolidColor.frag");
	if (fragmentShader == nullptr)
	{
		SDL_Log("Couldn't create fragment shader!");
		return false;
	}

	std::array vertexBufferDescriptions{
		SDL_GPUVertexBufferDescription{
			.slot = 0,
			.pitch = sizeof(Vertex),
			.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
			.instance_step_rate = 0,
		},
	};

	std::array vertexAttributes{
		SDL_GPUVertexAttribute{
			.location = 0,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
			.offset = 0 * sizeof(float),
		},
		SDL_GPUVertexAttribute{
			.location = 1,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
			.offset = 1 * sizeof(float),
		},
		SDL_GPUVertexAttribute{
			.location = 2,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT,
			.offset = 2 * sizeof(float),
		},
	};

	std::array colorTargetDescriptions{
		SDL_GPUColorTargetDescription{
			.format = SDL_GetGPUSwapchainTextureFormat(myAppState->device, myAppState->window)
		}
	};

	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = SDL_GPUGraphicsPipelineCreateInfo{
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
		.vertex_input_state = SDL_GPUVertexInputState{
			.vertex_buffer_descriptions = vertexBufferDescriptions.data(),
			.num_vertex_buffers = vertexBufferDescriptions.size(),
			.vertex_attributes = vertexAttributes.data(),
			.num_vertex_attributes = vertexAttributes.size(),
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = SDL_GPURasterizerState{
			.fill_mode = SDL_GPU_FILLMODE_FILL,
			.cull_mode = SDL_GPU_CULLMODE_BACK,
		},
		.target_info = SDL_GPUGraphicsPipelineTargetInfo{
			.color_target_descriptions = colorTargetDescriptions.data(),
			.num_color_targets = colorTargetDescriptions.size(),
		},
	};
	myAppState->pipeline = SDL_CreateGPUGraphicsPipeline(myAppState->device, &pipelineCreateInfo);
	if (myAppState->pipeline == nullptr)
	{
		SDL_Log("Couldn't create graphics pipeline! %s", SDL_GetError());
		return false;
	}

	SDL_ReleaseGPUShader(myAppState->device, vertexShader);
	SDL_ReleaseGPUShader(myAppState->device, fragmentShader);

	return true;
}

/// Returns true on success, false on failure
bool CreateVertexBuffer(MyAppState* myAppState, std::span<Vertex> vertices)
{
	myAppState->numVertices = vertices.size();
	Uint32 verticesSize = myAppState->numVertices * sizeof(Vertex);

	SDL_GPUBufferCreateInfo vertexBufferCreateInfo = SDL_GPUBufferCreateInfo{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = verticesSize,
	};

	myAppState->vertexBuffer = SDL_CreateGPUBuffer(myAppState->device, &vertexBufferCreateInfo);
	if (myAppState->vertexBuffer == nullptr)
	{
		SDL_Log("Couldn't create vertex buffer: %s", SDL_GetError());
		return false;
	}

	SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = SDL_GPUTransferBufferCreateInfo{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = verticesSize,
	};

	SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(myAppState->device, &transferBufferCreateInfo);
	if (transferBuffer == nullptr)
	{
		SDL_Log("Couldn't create transfer buffer: %s", SDL_GetError());
		return false;
	}

	Vertex* transferData = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(myAppState->device, transferBuffer, false));
	if (transferData == nullptr)
	{
		SDL_Log("Couldn't map transfer buffer: %s", SDL_GetError());
		SDL_ReleaseGPUTransferBuffer(myAppState->device, transferBuffer);
		return false;
	}

	SDL_memcpy(transferData, vertices.data(), verticesSize);

	SDL_UnmapGPUTransferBuffer(myAppState->device, transferBuffer);

	// Upload the transfer data to the vertex buffer
	SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(myAppState->device);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

	SDL_GPUTransferBufferLocation bufferLocation = SDL_GPUTransferBufferLocation{
		.transfer_buffer = transferBuffer,
		.offset = 0,
	};

	SDL_GPUBufferRegion bufferRegion = SDL_GPUBufferRegion{
		.buffer = myAppState->vertexBuffer,
		.offset = 0,
		.size = verticesSize,
	};

	SDL_UploadToGPUBuffer(copyPass, &bufferLocation, &bufferRegion, false);

	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
	SDL_ReleaseGPUTransferBuffer(myAppState->device, transferBuffer);
	return true;
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

	if (!CreatePipeline(myAppState))
	{
		// Error message already logged in the function
		return SDL_APP_FAILURE;
	}

	// Counter-Clockwise order:
	std::array vertices{
		Vertex{-1.0f, -1.0f, 0.0f}, // Bottom-Left
		Vertex{1.0f, -1.0f, 0.0f}, // Bottom-Right
		Vertex{0.0f, 1.0f, 0.0f}, // Top-Middle
	};
	if (!CreateVertexBuffer(myAppState, vertices))
	{
		// Error message already logged in the function
		return SDL_APP_FAILURE;
	}

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
		SDL_GPUBufferBinding bufferBinding = {
			.buffer = myAppState->vertexBuffer,
			.offset = 0,
		};
		SDL_BindGPUVertexBuffers(renderPass, 0, &bufferBinding, 1);
		SDL_DrawGPUPrimitives(renderPass, myAppState->numVertices, 1, 0, 0);

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

	SDL_ReleaseGPUBuffer(myAppState->device, myAppState->vertexBuffer);
	SDL_ReleaseGPUGraphicsPipeline(myAppState->device, myAppState->pipeline);

	SDL_ReleaseWindowFromGPUDevice(myAppState->device, myAppState->window);
	SDL_DestroyWindow(myAppState->window);
	SDL_DestroyGPUDevice(myAppState->device);

	delete myAppState;
}
