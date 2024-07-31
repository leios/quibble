cmake_minimum_required (VERSION 3.8)

project (Quibble LANGUAGES C)
find_package(OpenCL REQUIRED)

add_executable(
    quibble
    include/macros.h

    src/demo_functions.c
    include/demo_functions.h

    src/demo.c
)

target_link_libraries(quibble OpenCL::OpenCL)
