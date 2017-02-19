cmake_minimum_required(VERSION 3.5)
project(path_nrm)

set(CMAKE_C_STANDARD 99)

set(LIB_FILES path_nrm.h path_nrm.c)
set(TEST_FILES test.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "build/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "build/")

add_library(path_nrm ${LIB_FILES})
add_executable(path_nrm_test ${TEST_FILES})
target_link_libraries(path_nrm_test path_nrm)