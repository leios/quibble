cmake_minimum_required (VERSION 3.8)

project (Quibble LANGUAGES C)
find_package(OpenCL REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# QUIBBLE

add_executable(
    quibble
    include/macros.h

    include/errors.h
    src/errors.c

    include/quibble_buffers.h
    src/quibble_buffers.c

    include/quibble_canvas.h
    src/quibble_canvas.c

    include/demo_functions.h
    src/demo_functions.c

    src/demo.c
)

target_link_libraries(quibble OpenCL::OpenCL)

# QBINFO

add_executable(
    qbinfo
    include/macros.h

    include/errors.h
    src/errors.c

    include/quibble_buffers.h
    src/quibble_buffers.c

    include/quibble_canvas.h
    src/quibble_canvas.c

    src/qbinfo.c
)

target_link_libraries(qbinfo OpenCL::OpenCL)
