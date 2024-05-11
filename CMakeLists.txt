cmake_minimum_required(VERSION 3.8)

set(name Gui)
project(${name})

include(cmake/Configuration.cmake)
set(OutBinDir ${CMAKE_BINARY_DIR}/Bin)


add_subdirectory(thirdparty)

file(GLOB_RECURSE SOURCES_H
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
    
file(GLOB_RECURSE SOURCES_HPP 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
    
file(GLOB_RECURSE SOURCES_CPP 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    
file(GLOB_RECURSE SOURCES_CC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

file(GLOB_RECURSE SOURCES_C
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

set(_Sources
    ${SOURCES_H}
    ${SOURCES_HPP}
    ${SOURCES_CPP}
    ${SOURCES_CC}
    ${SOURCES_C}
)

source_group("" FILES ${_Sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${_Sources})

add_executable(${PROJECT_NAME} ${_Sources})
target_link_libraries(${PROJECT_NAME} thirdparty ${LUA_ENGINE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/thirdparty)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

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
