set(COMPILER_IS_MSVC OFF)
set(COMPILER_IS_GCC OFF)
set(COMPILER_IS_CLANG OFF)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
set(COMPILER_IS_MSVC ON)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
set(COMPILER_IS_GCC ON)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
set(COMPILER_IS_CLANG ON)
endif()

set(DEFAULT_COMPILE_DEFINITIONS)
set(DEFAULT_COMPILE_OPTIONS)
set(DEFAULT_LINKER_OPTIONS)

if (COMPILER_IS_MSVC)
    set(DEFAULT_COMPILE_DEFINITIONS ${DEFAULT_COMPILE_DEFINITIONS} 
        _SCL_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the Standard C++ Library
        _CRT_SECURE_NO_WARNINGS  # Calling any one of the potentially unsafe methods in the CRT Library
    )

    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
    PRIVATE
        /MP           # -> build with multiple processes
        /W3           # -> warning level 3
        /wd4251       # -> disable warning: 'identifier': class 'type' needs to have dll-interface to be used by clients of class 'type2'
        /wd4592       # -> disable warning: 'identifier': symbol will be dynamically initialized (implementation limitation)      
      
        PUBLIC
    )

	set(DEFAULT_LINKER_OPTIONS ${DEFAULT_LINKER_OPTIONS} shlwapi.lib)
endif ()

# GCC and Clang compiler options
if (COMPILER_IS_GCC OR COMPILER_IS_CLANG)
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
    PRIVATE
        #-fno-exceptions # since we use stl and stl is intended to use exceptions, exceptions should not be disabled

        -Wall
        -Wextra
        -Wunused

        -Wreorder
        -Wignored-qualifiers
        -Wmissing-braces
        -Wreturn-type
        -Wswitch
        -Wswitch-default
        -Wuninitialized
        -Wmissing-field-initializers
        
        $<$<CXX_COMPILER_ID:GNU>:
            -Wmaybe-uninitialized
        
            -Wno-unknown-pragmas
            
            $<$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,4.8>:
                -Wpedantic
                
                -Wreturn-local-addr
            >
        >
        
        $<$<CXX_COMPILER_ID:Clang>:
            -Wpedantic
                
            # -Wreturn-stack-address # gives false positives
        >
    PUBLIC
        $<$<PLATFORM_ID:Darwin>:
            -pthread
        >
    )
endif ()

# Use pthreads on mingw and linux
if(COMPILER_IS_GCC OR IS_LINUX)
    set(DEFAULT_LINKER_OPTIONS
    PUBLIC
        -pthread
    )
endif()