cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_COMPILER clang)
project (Quibble LANGUAGES C)
find_package(OpenCL REQUIRED)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-varargs)
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

    include/io.h
    src/io.c

    include/quibble_args.h
    src/quibble_args.c

    include/quibble_program.h
    src/quibble_verses.c
    src/quibble_stanzas.c
    src/quibble_poems.c
    src/quibble_program.c

    test/tests.h
    test/io_tests.c
    test/program_tests.c

    src/demo.c
)

target_link_libraries(quibble m OpenCL::OpenCL)

# QBINFO

add_executable(
    qbinfo

    include/errors.h
    src/errors.c

    include/qbinfo.h
    src/qbinfo.c
)

target_link_libraries(qbinfo OpenCL::OpenCL)

execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/configure)
