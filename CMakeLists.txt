cmake_minimum_required(VERSION 3.20)
project(TextToEpub)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

#google test
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#json
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.10.4
)

FetchContent_GetProperties(json)

#update json
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

enable_testing()

file(GLOB UTIL_SRC "util/*.h" "util/*.cpp")

#test toggle
set(TEST ON)

if (TEST)
    set(MAIN_SRC test/test.cpp)
    else()
    set(MAIN_SRC main.cpp)
endif ()

add_executable(TextToEpub
        ${MAIN_SRC}
        ${UTIL_SRC}
        )

include(GoogleTest json)
target_link_libraries(TextToEpub
        gtest_main nlohmann_json::nlohmann_json
        )

gtest_discover_tests(TextToEpub)