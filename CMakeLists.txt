cmake_minimum_required(VERSION 3.4)

project(audio_benchmarks)

#Force OpenALSoft options to appropriate values.
set(ALSOFT_EXAMPLES OFF CACHE BOOL "No OpenALSoft examples.")
set(ALSOFT_TESTS OFF CACHE BOOL "No OpenALSoft tests.")
SET(LIBTYPE STATIC CACHE STRING "Make OpenALSoft a static library.")

add_subdirectory(openal-soft)

add_executable(openal_benchmark
benchmarks/OpenAL/main.cpp
)
link_libraries(openal_benchmark openal_static)
