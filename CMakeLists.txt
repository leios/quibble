cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_COMPILER clang)
project (Quibble LANGUAGES C)
find_package(OpenCL REQUIRED)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-varargs)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# QUIBBLE

add_library(
    quibble SHARED

    include/quibble.h

    src/errors.c
    src/string_manip.c
    src/quibble_args.c
    src/images.c
    src/config.c
    src/quibble_verses.c
    src/quibble_stanzas.c
    src/quibble_poems.c
    src/quibble_program.c
    src/camera.c

)

target_link_libraries(quibble m OpenCL::OpenCL)

# QBTEST

add_executable(
    qbtest

    test/tests.h
    test/io_tests.c
    test/program_tests.c

    test/test.c
)

target_link_libraries(qbtest m -L${CMAKE_BINARY_DIR}/lib quibble OpenCL::OpenCL)

# QBINFO

add_executable(
    qbinfo

    include/quibble.h
    src/errors.c

    include/qbinfo.h
    src/qbinfo.c
)

target_link_libraries(qbinfo OpenCL::OpenCL)

install(TARGETS quibble)
install(TARGETS qbinfo)
install(FILES include/quibble.h DESTINATION /usr/local/include)

execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/configure)
