project(Kingkong)

cmake_minimum_required(VERSION 3.5...3.16)


SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/build/CMakeModules/;${CMAKE_MODULE_PATH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
macro(set_target_folder_if_exist target folder)
	if (TARGET ${target})
		set_target_properties(${target} PROPERTIES FOLDER ${folder})
	endif()
endmacro()


add_subdirectory(base)

if (ENABLE_MVVM)
	add_subdirectory(mvvm)
endif()

add_subdirectory(third_party)
add_subdirectory(tool)
if (ENABLE_UI)
	add_subdirectory(app)
endif()










