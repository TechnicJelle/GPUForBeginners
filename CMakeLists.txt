cmake_minimum_required(VERSION 3.31)
project(GPU_Programming_With_SDLGPU)

# Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Libraries

## SDL3
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Projects
add_subdirectory(chapters/chapter01)

# Copy assets
## Adapted from https://github.com/Moros1138/pge-template-project/blob/fb5c4bf23ececd2cc9eee880fd8a2e7fa05c2ece/CMakeLists.txt#L271-L287
set(ASSETS_DIR "assets")
set(SOURCE_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR})
set(DATA_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ASSETS_DIR})

file(GLOB_RECURSE src_data_files
		RELATIVE ${SOURCE_DATA_DIR}/ "${SOURCE_DATA_DIR}/*.*" "${SOURCE_DATA_DIR}/*")
foreach (fn ${src_data_files})
	add_custom_command(
			OUTPUT ${DATA_OUTPUT_DIR}/${fn}
			COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DATA_DIR}/${fn} ${DATA_OUTPUT_DIR}/${fn}
			MAIN_DEPENDENCY ${SOURCE_DATA_DIR}/${fn})
	list(APPEND out_data_files ${DATA_OUTPUT_DIR}/${fn})
endforeach ()

add_custom_target(copy_assets DEPENDS ${out_data_files})
