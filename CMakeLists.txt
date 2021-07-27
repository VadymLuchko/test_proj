cmake_minimum_required(VERSION 3.5)

project(test_proj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES 
    thread/thread_wrapper.h)

add_executable(test_proj main.cpp ${HEADER_FILES})

target_link_libraries(test_proj pthread)
