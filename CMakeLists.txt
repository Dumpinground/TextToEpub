cmake_minimum_required(VERSION 3.20)
project(TextToEpub)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

include(FetchContent)

add_subdirectory(boost-cmake)
add_subdirectory(thirdparty)
add_subdirectory(argument)
add_subdirectory(src)
add_subdirectory(test)
