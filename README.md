# GPU Programming with SDL's GPU API

or: **How To Get Started With GPU Programming for Beginners, With SDL3's GPU API.**

This repository contains both the guide/book in the [`docs`](docs) directory,
as well as the example code that you can clone and run and look at when you're stuck.

## [Click here to start reading the guide](docs/README.md)

## Running the examples

First, clone this repository recursively:

```sh
git clone --recursive https://github.com/TechnicJelle/GPUForBeginners.git
```

or

```sh
git clone --recursive git@github.com:TechnicJelle/GPUForBeginners.git
```

Then, either open the directory in an IDE and let it import.
You can then select the chapter you want to see and click the Run button.

You can also run `cmake` from the command-line instead of using an IDE:

```sh
cmake -S . -B build -G Ninja
cmake --build build --parallel
```

You will then find the executables in the `build/` directory.

