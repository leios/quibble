cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_COMPILER clang)
project (Quibble LANGUAGES C)
find_package(OpenCL REQUIRED)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-varargs -fsanitize=address)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# QUIBBLE

add_executable(
    quibble
    include/macros.h

    include/errors.h
    src/errors.c

    include/quibble_inputs.h
    src/quibble_inputs.c

    include/io.h
    src/io.c

    include/quibble_canvas.h
    src/quibble_canvas.c

    include/quibble_program.h
    src/quibble_program.c

    include/demo_functions.h
    src/demo_functions.c

    test/tests.h
    test/io_tests.c
    test/program_tests.c

    src/demo.c
)

target_link_libraries(quibble asan OpenCL::OpenCL)

# QBINFO

add_executable(
    qbinfo
    include/macros.h

    include/errors.h
    src/errors.c

    include/quibble_inputs.h
    src/quibble_inputs.c

    include/quibble_canvas.h
    src/quibble_canvas.c

    src/qbinfo.c
)

target_link_libraries(qbinfo asan OpenCL::OpenCL)
