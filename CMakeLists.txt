cmake_minimum_required (VERSION 3.8)

project (Quibble LANGUAGES C)
find_package(OpenCL REQUIRED)

add_executable(
    quibble
    include/macros.h

    include/errors.h
    src/errors.c

    include/quibble_context.h
    src/quibble_context.c

    include/quibble_buffers.h
    src/quibble_buffers.c

    include/demo_functions.h
    src/demo_functions.c

    src/demo.c
)

target_link_libraries(quibble OpenCL::OpenCL)
