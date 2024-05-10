
# set friendly platform define
 if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
     set(WINDOWS TRUE)
     set(SYSTEM_STRING "Windows Desktop")
     if(NOT ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32"))
        set(WIN64 TRUE)
        set(ARCH_ALIAS "x64")
     else()
        set(WIN32 TRUE)
        set(ARCH_ALIAS "x86")
     endif()
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
     set(SYSTEM_STRING "Android")
     set(ARCH_ALIAS ${ANDROID_ABI})
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
     if(ANDROID)
         set(SYSTEM_STRING "Android")
     else()
         set(LINUX TRUE)
         set(SYSTEM_STRING "Linux")
     endif()
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
     if(IOS)
         set(APPLE TRUE)
         set(SYSTEM_STRING "IOS")
     else()
         set(APPLE TRUE)
         set(MACOSX TRUE)
         set(SYSTEM_STRING "Mac OSX")
     endif()
 endif()

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_FLAG -std=c++20)

#if(MSVC)     
#    # Use the static C library for all build types
#    foreach(var 
#        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
#        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
#        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
#        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
#      )
#      if(${var} MATCHES "/MD")
#        string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
#      endif()
#    endforeach()    
#endif(MSVC)
