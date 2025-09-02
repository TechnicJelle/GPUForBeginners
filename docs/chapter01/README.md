# Chapter 1: Setup

Create a new C++ project in whatever way you prefer.

Then add SDL to it, by following the installation instructions on SDL's page itself:
[INSTALL.md](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md)

Now I assume that you have followed that and now have a `main.cpp` C++ file that compiles and links against SDL3.

I will use CMake for this guide, as it is what SDL itself uses.
It also ensures the best cross-platform and cross-editor functionality and compatibility.

## Game Loop

SDL3 provides a simple, highly cross-platform way to create a game loop,
through its "[Main Callbacks](https://wiki.libsdl.org/SDL3/README-main-functions)" system.
In this guide, we will use that to keep our code simple and portable.

So remove any existing code in your `main.cpp` file, and replace it with the following:

```c++
#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

// This function runs once at startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	SDL_Log("Init");
	// Continue running the application
	return SDL_APP_CONTINUE;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	SDL_Log("Event");
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
	SDL_Log("Iterate");
	// Continue running the application
	return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	SDL_Log("Quit");
}
```

This will log a message "Init" once, at the start of the application,
"Event" every time an event like mouse or keyboard input comes in (many times per frame),
"Iterate" once every single frame, and "Quit" once at the end.

In the next section, we will start replacing these logs with actual code.

## Window

Before we start by creating the window in which we will draw all our stuff,
we first need a place to store the pointer to that window.

Above the callback functions, create a struct:

```c++
struct MyAppState
{
	SDL_Window* window = nullptr;
};
```

Now, in the `SDL_AppInit` function, create an instance of it on the heap and pass it to SDL for safekeeping:

```c++
MyAppState* myAppState = new MyAppState();
*appstate = myAppState;
```

We can then create the window and store it in our struct instance:

```c++
myAppState->window = SDL_CreateWindow("Hello, SDL GPU!", 1280, 720, 0);
if (myAppState->window == nullptr)
{
	SDL_Log("Couldn't create window: %s", SDL_GetError());
	return SDL_APP_FAILURE;
}
```

Don't forget to check for errors!

If you run the application now, you will see a blank/black/empty screen that you can close with the X button.

You should probably remove the `SDL_Log`s, now.

In the next chapter, we will color in the window with the GPU!

[Index](../README.md) | [Next Chapter →](../chapter02/README.md)
