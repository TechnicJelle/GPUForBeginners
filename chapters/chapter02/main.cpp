#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

struct MyAppState
{
	SDL_Window* window = nullptr;
	SDL_GPUDevice* device = nullptr;
};

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

	SDL_ReleaseWindowFromGPUDevice(myAppState->device, myAppState->window);
	SDL_DestroyGPUDevice(myAppState->device);
	SDL_DestroyWindow(myAppState->window);

	delete myAppState;
}
