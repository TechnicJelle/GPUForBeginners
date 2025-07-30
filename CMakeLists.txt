cmake_minimum_required(VERSION 3.31)
project(GPU_Programming_With_SDLGPU)

#TODO: Don't do this, because it will make using assets more complicated.
#Each chapter should be in its own directory, and the assets should be in a subdirectory of that.
#And SDL should probably be copied (or at least symlinked?) to each chapter directory.

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Libraries

## SDL3
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Projects
add_subdirectory(example/chapter01)
