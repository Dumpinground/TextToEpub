include(FetchContent)

FetchContent_Declare(
        xml
        GIT_REPOSITORY https://github.com/zeux/pugixml
        GIT_TAG v1.13
)

FetchContent_GetProperties(xml)

if(NOT xml_POPULATED)
    FetchContent_Populate(xml)
    add_subdirectory(${xml_SOURCE_DIR} ${xml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
