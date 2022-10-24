cmake_minimum_required(VERSION 3.13)
project(minimc-gui)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/minimc/cmake_modules;${CMAKE_MODULE_PATH}")
set (CMAKE_CXX_STANDARD 20)

enable_testing()

add_compile_options(-Wall -Wextra -Wpedantic)
if(MINIMC32PTr)
    add_compile_definitions(MINIMC32)
endif()

SET (ToolName minimc-gui)
SET (VERSION_MAJOR 0)
SET (VERSION_MINOR "1")
execute_process(COMMAND
        git describe --always --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_HASH
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

add_subdirectory (dependencies)
add_subdirectory (minimc)
add_subdirectory(binding app)

configure_file (setup.py.in setup.py)
