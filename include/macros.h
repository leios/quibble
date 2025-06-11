#ifndef MACROS_H
#define MACROS_H

// These numbers are arbitrarily set
// #define MAX_SOURCE_SIZE (0x100000)
#define MAX_SOURCE_SIZE (0x999999)
#define MAX_PREAMBLE_SIZE (0x100000)

// I might want to ise 1.2 in the end...
#define CL_TARGET_OPENCL_VERSION 300
//#define CL_TARGET_OPENCL_VERSION 120

// Delay Compilation
#define QBINLINE(...) "// QBINLINE GENERATED\n" #__VA_ARGS__ "\0"

#endif
