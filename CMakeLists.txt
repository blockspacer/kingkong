project(Kingkong)

cmake_minimum_required(VERSION 3.5...3.16)


SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/build/CMakeModules/;${CMAKE_MODULE_PATH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)

add_subdirectory(base)
add_subdirectory(mvvm)
add_subdirectory(third_party)










