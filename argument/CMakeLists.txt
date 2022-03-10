set(BINARY ${CMAKE_PROJECT_NAME}_argument)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.c *.cpp)

add_library(${BINARY}_lib STATIC ${SOURCES})