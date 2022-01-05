cmake_minimum_required(VERSION 3.20)
project(TextToEpub)

set(CMAKE_CXX_STANDARD 20)

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

#xml
FetchContent_Declare(
        xml
        GIT_REPOSITORY https://github.com/zeux/pugixml
        GIT_TAG v1.11.4
)

FetchContent_GetProperties(xml)

if(NOT xml_POPULATED)
    FetchContent_Populate(xml)
    add_subdirectory(${xml_SOURCE_DIR} ${xml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

#Boost
#FetchContent_Declare(
#        Boost
#        GIT_REPOSITORY https://github.com/boostorg/boost_install
#        GIT_TAG boost-1.77.0
#)
#
#FetchContent_GetProperties(Boost)

add_subdirectory(boost-cmake)

enable_testing()

set(UTIL_SRC
        util/util.h util/util.cpp
        util/Book.h util/Book.cpp
        util/Context.h util/Context.cpp
        util/uuid.h util/uuid.cpp
        util/outline.h argument/argument.h argument/argument.cpp)
set(Test_SRC
        test/test.h
        test/test.cpp
        test/book.cpp)
set(ThirdParty_SRC
        thirdparty/gpakosz_uuid4/uuid4.c
        thirdparty/gpakosz_uuid4/uuid4.h)

#test toggle
set(TEST OFF)

if (TEST)
    set(MAIN_SRC ${Test_SRC})
    else()
    set(MAIN_SRC main.cpp)
endif ()

add_executable(TextToEpub
        ${MAIN_SRC}
        ${UTIL_SRC}
        ${ThirdParty_SRC}
        )

include(GoogleTest json)
target_link_libraries(TextToEpub PRIVATE
        gtest_main nlohmann_json::nlohmann_json pugixml::static
        )

target_link_libraries(TextToEpub PUBLIC Boost::filesystem)
target_link_libraries(TextToEpub PUBLIC Boost::boost)

gtest_discover_tests(TextToEpub)