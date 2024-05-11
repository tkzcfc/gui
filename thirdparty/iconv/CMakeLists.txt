
set(lib_name iconv)
set(target_name iconv)

project(${lib_name})


include(../cmake/ConfigThirdparty.cmake)

add_library(${target_name} STATIC iconv.c localcharset.c relocatable.c)

target_compile_definitions(${target_name} 
    PRIVATE _CRT_SECURE_NO_WARNINGS 
    PUBLIC USING_STATIC_LIBICONV
    )

target_include_directories(${target_name}
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

