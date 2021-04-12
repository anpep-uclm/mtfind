cmake_minimum_required(VERSION 3.19)
project(mtfind)

set(CMAKE_CXX_STANDARD 17)

add_executable(mtfind src/Thread.cpp src/SearchThread.cpp src/mtfind.cpp include/MTFind/SearchResult.h include/MTFind/SearchState.h)
target_include_directories(mtfind PRIVATE include)