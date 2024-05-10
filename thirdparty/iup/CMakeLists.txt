
set(lib_name iup)
set(target_name dep_${lib_name})

project(${lib_name})


add_library(${target_name} STATIC IMPORTED GLOBAL)




set(IUP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE STRING "iup include dir" FORCE)

if(WINDOWS)
  # iup-3.31-Lua54_Win64_vc17_lib

  set_target_properties(${target_name} PROPERTIES
    IMPORTED_LOCATION "${platform_spec_path}/iuplua54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluacd54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluacontrols54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluafiledlg54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluagl54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluaglcontrols54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluaim54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluaimglib54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluaole54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluascripterdlg54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluatuio54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iupluaweb54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iuplua_mglplot54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iuplua_plot54.lib"
    IMPORTED_LOCATION "${platform_spec_path}/iuplua_scintilla54.lib"
  )
else()
  set_target_properties(${target_name} PROPERTIES
    IMPORTED_LOCATION "${platform_spec_path}/libxxx.a"
  )
endif()

set_target_properties(${target_name} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

