set(lib_name rust_net)
set(target_name dep_${lib_name})

project(${lib_name})

include(../cmake/ConfigThirdparty.cmake)

add_library(${target_name} STATIC IMPORTED GLOBAL)


if(WINDOWS)
  set_target_properties(${target_name} PROPERTIES
    IMPORTED_LOCATION "${platform_spec_path}/${lib_name}.lib"
  )

  target_link_libraries(
    ${target_name}
    INTERFACE Crypt32
    INTERFACE ws2_32
    INTERFACE Bcrypt
    INTERFACE Userenv
    INTERFACE Ntdll
    INTERFACE Secur32
    Ncrypt
  )          
else()
  set_target_properties(${target_name} PROPERTIES
    IMPORTED_LOCATION "${platform_spec_path}/lib${lib_name}.a"
  )
endif()

set_target_properties(${target_name} PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

