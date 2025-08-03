cmake_minimum_required(VERSION 3.31)
project(GPU_Programming_With_SDLGPU)

# Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Libraries

## SDL3
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Copy assets
## Adapted from https://github.com/Moros1138/pge-template-project/blob/fb5c4bf23ececd2cc9eee880fd8a2e7fa05c2ece/CMakeLists.txt#L271-L287
set(ASSETS_DIR "assets")
set(SOURCE_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ASSETS_DIR})
set(ASSETS_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ASSETS_DIR})

file(GLOB_RECURSE src_assets_files
		RELATIVE ${SOURCE_ASSETS_DIR}/
		"${SOURCE_ASSETS_DIR}/*.*" "${SOURCE_ASSETS_DIR}/*")
foreach (fn ${src_assets_files})
	set(out_f ${ASSETS_OUTPUT_DIR}/${fn})
	add_custom_command(
			OUTPUT ${out_f}
			COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_ASSETS_DIR}/${fn} ${out_f}
			MAIN_DEPENDENCY ${SOURCE_ASSETS_DIR}/${fn}
	)
	list(APPEND out_assets_files ${out_f})
endforeach ()

add_custom_target(copy_assets DEPENDS ${out_assets_files})

# Macro for Compiling Shaders (will be called in each chapter's CMakeLists.txt)
macro(compile_shaders)
	set(SHADERS_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)

	file(GLOB_RECURSE src_shader_files
			RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/
			"${CMAKE_CURRENT_SOURCE_DIR}/*.hlsl")

	# Find folders that could contain shadercross
	file(GLOB SC_DIRS "${CMAKE_SOURCE_DIR}/SDL*_shadercross*")
	find_program(SC_EXECUTABLE shadercross REQUIRED
			HINTS ${SC_DIRS}
			PATH_SUFFIXES bin)

	foreach (fn ${src_shader_files})
		set(in_f "${CMAKE_CURRENT_SOURCE_DIR}/${fn}")
		set(out_f "${SHADERS_OUTPUT_DIR}/${fn}")
		string(REPLACE ".hlsl" ".spv" fn_spv ${out_f})
		string(REPLACE ".hlsl" ".msl" fn_msl ${out_f})
		string(REPLACE ".hlsl" ".dxil" fn_dxil ${out_f})
		add_custom_command(
				OUTPUT ${out_f}
				COMMAND ${SC_EXECUTABLE} ${in_f} -o ${fn_spv}
				COMMAND ${SC_EXECUTABLE} ${in_f} -o ${fn_msl}
				COMMAND ${SC_EXECUTABLE} ${in_f} -o ${fn_dxil}
				MAIN_DEPENDENCY ${in_f}
		)
		list(APPEND out_shader_files ${out_f})
	endforeach ()

	add_custom_target(${PROJECT_NAME}_shaders DEPENDS ${out_shader_files})

	add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_shaders)
endmacro()

# Projects
add_subdirectory(chapters/chapter01)
add_subdirectory(chapters/chapter02)
