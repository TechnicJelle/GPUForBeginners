# GPU Programming with SDL's GPU API

or: **How To Get Started With GPU Programming for Beginners, With SDL3's GPU API.**

This repository contains both the guide/book in the [`docs`](docs) directory,
as well as the final code for each chapter that you can look at when you're stuck,
and also clone and run yourself.

## [Click here to start reading the guide](docs/README.md)

## Running the chapters

### 1. Cloning

First, clone this repository recursively:

```sh
git clone --recursive https://github.com/TechnicJelle/GPUForBeginners.git
```

or

```sh
git clone --recursive git@github.com:TechnicJelle/GPUForBeginners.git
```

### 2. Shadercross

Now that the code has been downloaded, you also need the "SDL_shadercross" shader compiler.

You can download it from the Actions tab here:
[github.com/libsdl-org/SDL_shadercross](https://github.com/libsdl-org/SDL_shadercross)  
Make sure to get the latest successful build and to download the artifact for your specific platform.  

#### Option A: System

You can unpack the archive you downloaded somewhere and add its `bin/` directory to your PATH.
CMake should then automatically detect and use it.

#### Option B: Project

Alternatively, you can unpack the archive into the root of this repository.
CMake should then automatically detect and use it.

### 3. Build

#### Option A: Editor (IDE)

Then, either open the directory in an IDE and let it import.
You can then select the chapter you want to see and click the Run button.

#### Option B: Command-line (Terminal)

You can also run `cmake` from the command-line instead of using an IDE:

```sh
cmake -S . -B build -G Ninja
cmake --build build --parallel
```

_(Ninja seems to be required, as normal Unix Makefiles fail to build.)_

Or build a single chapter:

```sh
cmake --build build --target Chapter01
```

You will then find the executables in the `build/` directory.

