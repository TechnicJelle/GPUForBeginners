cmake_minimum_required(VERSION 3.31)
project(Chapter01)

# Creates the TARGET for this project, which is the executable.
# The commands after that require the TARGET to be created first.

## The WIN32 option is used to create a window without a console on Windows.
## It is safe to use on other platforms as well, where it won't do anything
add_executable(${PROJECT_NAME} WIN32 main.cpp)

# This chapter does not have any assets or shaders, so we don't need to copy or compile anything.

# Set C++ version
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Link to Libraries

## SDL3
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
