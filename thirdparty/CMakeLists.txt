project(thirdparty)

option(GUI_WITH_UNZIP "Build with internal unzip support" ON)
option(GUI_WITH_CONVERTUTF "Build with internal ConvertUTF support" ON)

function(configure_target_outdir target)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        FOLDER "ThirdParty"
    )
endfunction()

add_library(thirdparty STATIC empty.cpp)

add_subdirectory(fmt EXCLUDE_FROM_ALL)
target_link_libraries(thirdparty fmt-header-only)

add_subdirectory(zlib)
target_link_libraries(thirdparty
    dep_zlib
)

if(GUI_WITH_CONVERTUTF)
    add_subdirectory(ConvertUTF)
    target_link_libraries(thirdparty ConvertUTF)
    configure_target_outdir(ConvertUTF)
endif(GUI_WITH_CONVERTUTF)

set(LUA_ENGINE plainlua)
add_subdirectory(lua/${LUA_ENGINE})
add_subdirectory(lua/lua-cjson)
add_subdirectory(lua/luasocket)
target_link_libraries(thirdparty ${LUA_ENGINE} lua-cjson luasocket)
target_include_directories(thirdparty PUBLIC ${CMAKE_CURRENT_LIST_DIR}/lua)


# unzip depend on zlib
if(GUI_WITH_UNZIP)
    add_subdirectory(unzip)
    target_link_libraries(thirdparty unzip)
    configure_target_outdir(unzip)
endif(GUI_WITH_UNZIP)


# put "thirdparty" into thirdparty folder, too
configure_target_outdir(thirdparty)
