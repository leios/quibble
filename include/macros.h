#ifndef MACROS_H
#define MACROS_H

//#define MAX_SOURCE_SIZE (0x100000)
#define MAX_SOURCE_SIZE (0x999999)

#define MAX_PREAMBLE_SIZE (0x100000)

#define CL_TARGET_OPENCL_VERSION 300
//#define CL_TARGET_OPENCL_VERSION 120

// Delay Compilation
#define DCOMPILE(...) ({ \
    char str[MAX_PREAMBLE_SIZE/8] = "// DCOMPILE GENERATED\n"; \
    strcat(str, #__VA_ARGS__); \
})

#endif
