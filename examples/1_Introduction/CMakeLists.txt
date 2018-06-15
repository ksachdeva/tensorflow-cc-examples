set(INTRODUCTION_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(INTRODUCTION_SRC_PATH  "${INTRODUCTION_PATH}/src" )

# this macro will help create the executables
# for various introduction applications
macro(add_introduction app_name app_path main_file)
    add_executable(intro-${app_name} ${app_path}/${main_file}.cc)
    target_link_libraries(
        intro-${app_name}
        PRIVATE
        third-party::tensorflow-cc
    )
endmacro()

# here simply call the macro for every sample file (the one that contains the main)
add_introduction(hello-world ${INTRODUCTION_SRC_PATH} hello-world)
add_introduction(basic-operations ${INTRODUCTION_SRC_PATH} basic-operations)
add_introduction(basic-tensors ${INTRODUCTION_SRC_PATH} basic-tensors)


