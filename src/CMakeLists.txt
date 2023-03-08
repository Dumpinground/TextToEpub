set(BINARY ${CMAKE_PROJECT_NAME}_run)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

add_executable(${BINARY} ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

set(EX_LIB_LIST
        nlohmann_json::nlohmann_json
        pugixml::static
#        Boost::boost Boost::filesystem
        ${CMAKE_PROJECT_NAME}_third_party_lib
        ${CMAKE_PROJECT_NAME}_argument_lib
        )

target_link_libraries(${BINARY} PRIVATE
        ${EX_LIB_LIST}
        )

target_link_libraries(${BINARY}_lib PRIVATE
        ${EX_LIB_LIST}
        )
