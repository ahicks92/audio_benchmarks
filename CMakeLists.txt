cmake_minimum_required(VERSION 3.4)

project(audio_benchmarks)

#Force OpenALSoft options to appropriate values.
set(ALSOFT_EXAMPLES OFF CACHE BOOL "No OpenALSoft examples.")
set(ALSOFT_TESTS OFF CACHE BOOL "No OpenALSoft tests.")


add_subdirectory(openal-soft)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/openal-soft/include")

add_executable(openal_benchmark
benchmarks/OpenAL/main.cpp
)
target_link_libraries(openal_benchmark openal_soft)

set(DEST "${CMAKE_CURRENT_BINARY_DIR}")

set_target_properties(openal_soft
PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${DEST}"
LIBRARY_OUTPUT_DIRECTORY "${DEST}"
RUNTIME_OUTPUT_DIRECTORY "${DEST}")