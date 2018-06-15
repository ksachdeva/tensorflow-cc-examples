if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(IS_WINDOWS ON)    
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(IS_OSX ON)    
endif()

if (${CMAKE_SYSTEM} STREQUAL "Linux-4.4.38-tegra")
    set(IS_JETSON ON)    
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(IS_LINUX ON)    
endif()


