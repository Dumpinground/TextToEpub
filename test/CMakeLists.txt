set(BINARY ${CMAKE_PROJECT_NAME}_test)

#google test
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f2d71f0be2ff2230789cfaf37d4c4f38db5b1c7c.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

add_executable(${BINARY} ${TEST_SOURCES})

enable_testing()

include(GoogleTest)
include(json)
include(xml)

target_link_libraries(${BINARY} PRIVATE
        gtest_main
        nlohmann_json::nlohmann_json
        pugixml::static
#        Boost::boost Boost::filesystem
        ${CMAKE_PROJECT_NAME}_run_lib
        )

gtest_discover_tests(${BINARY})
