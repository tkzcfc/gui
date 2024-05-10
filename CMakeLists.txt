cmake_minimum_required(VERSION 3.8)

set(name Gui)
project(${name})

include(cmake/Configuration.cmake)

add_subdirectory(thirdparty)

set(_Sources
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

source_group("" FILES ${_Sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_Sources})

add_executable(${PROJECT_NAME} ${_Sources})
target_link_libraries(${PROJECT_NAME} thirdparty)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/thirdparty)

set(OutBinDir ${CMAKE_BINARY_DIR}/Bin)
set_target_properties(${PROJECT_NAME} PROPERTIES
    FOLDER "Apps"
    RUNTIME_OUTPUT_DIRECTORY                "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${OutBinDir}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${OutBinDir}"
    DEBUG_POSTFIX                           _d
    RELWITHDEBINGO_POSTFIX                  _rd
    MINSIZEREL_POSTFIX                      _r
    VS_DEBUGGER_WORKING_DIRECTORY           ${OutBinDir}
)
