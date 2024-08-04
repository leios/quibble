/*-------------errors.c-------------------------------------------------------//

Purpose: This file defines error checking for OpenCL

//----------------------------------------------------------------------------*/
#include "../include/errors.h"

void cl_check(cl_int error){
    if (error != CL_SUCCESS){
        fprintf(stderr, "%s\nError number: %d\n",
                        get_cl_error_string(error), error);
        exit(1);
    }
}

void cl_check_program(cl_int error, cl_program program, cl_device_id device_id){
    if (error != CL_SUCCESS){
        fprintf(stderr, "%s\nError number: %d\n",
                        get_cl_error_string(error), error);
        size_t log_size = 0;

        // Fuinding length of program
        cl_check(
            clGetProgramBuildInfo(program,
                                  device_id,
                                  CL_PROGRAM_BUILD_LOG,
                                  0,
                                  NULL,
                                  &log_size)
        );

        // populating buffer
        char *buffer = calloc(log_size, sizeof(char));
        cl_check(
            clGetProgramBuildInfo(program,
                                  device_id,
                                  CL_PROGRAM_BUILD_LOG,
                                  log_size,
                                  buffer,
                                  NULL)
        );
        printf("%s\n", buffer);
        exit(1);
    }
}

const char *get_cl_error_string(cl_int error)
{
    switch(error){
        case CL_SUCCESS: break;
        case CL_BUILD_PROGRAM_FAILURE:
            return "CL_BUILD_PROGRAM_FAILURE:\nclBuildProgram failed to build the specified program.";
            break;
        case CL_COMPILE_PROGRAM_FAILURE:
            return "CL_COMPILE_PROGRAM_FAILURE:\nclCompileProgram failed to compile the specified program.";
            break;
        case CL_COMPILER_NOT_AVAILABLE:
            return "CL_COMPILER_NOT_AVAILABLE:\nCL_DEVICE_COMPILER_AVAILABLE is CL_FALSE.";
            break;
        case CL_DEVICE_NOT_FOUND:
            return "CL_DEVICE_NOT_FOUND:\nNo devices were found that match the specified device type.";
            break;
        case CL_DEVICE_NOT_AVAILABLE:
            return "CL_DEVICE_NOT_AVAILABLE:\nCL_DEVICE_AVAILABLE is CL_FALSE.";
            break;
        case CL_DEVICE_PARTITION_FAILED:
            return "CL_DEVICE_PARTITION_FAILED:\ndevice partitioning is supported but the device\ncould not be further partitioned.";
            break;
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:\nevent in the event wait list has a negative value.";
            break;
        case CL_IMAGE_FORMAT_MISMATCH:
            return "CL_IMAGE_FORMAT_MISMATCH:\nattempting to copy images that do not use the same image format.";
            break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            return "CL_IMAGE_FORMAT_NOT_SUPPORTED:\nattempting to create or use an image format that is not supported.";
            break;
        case CL_INVALID_ARG_INDEX:
            return "CL_INVALID_ARG_INDEX:\nattempting to get or set a kernel argument using an invalid index for the specified kernel.";
            break;
        case CL_INVALID_ARG_SIZE:
            return "CL_INVALID_ARG_SIZE:\nthe specified size of a kernel argument does not match the size of the kernel argument.";
            break;
        case CL_INVALID_ARG_VALUE:
            return "CL_INVALID_ARG_VALUE:\nattempting to set a kernel argument that is not valid.";
            break;
        case CL_INVALID_BINARY:
            return "CL_INVALID_BINARY:\na program binary is not valid for a device.";
            break;
        case CL_INVALID_BUFFER_SIZE:
            return "CL_INVALID_BUFFER_SIZE:\nattempting to create a buffer or a sub-buffer with an invalid size.";
            break;
        case CL_INVALID_BUILD_OPTIONS:
            return "CL_INVALID_BUILD_OPTIONS:\nbuild options passed to clBuildProgram are not valid.";
            break;
        case CL_INVALID_COMMAND_QUEUE:
            return "CL_INVALID_COMMAND_QUEUE:\nthe specified command-queue is not a valid command-queue.";
            break;
        case CL_INVALID_COMPILER_OPTIONS:
            return "CL_INVALID_COMPILER_OPTIONS:\ncompiler options passed to clCompileProgram are not valid.";
            break;
        case CL_INVALID_CONTEXT:
            return "CL_INVALID_CONTEXT:\na specified context is not a valid context, or when mixing objects from multiple contexts.";
            break;
        case CL_INVALID_DEVICE:
            return "CL_INVALID_DEVICE:\na specified device is not a valid device.";
            break;
        case CL_INVALID_DEVICE_PARTITION_COUNT:
            return "CL_INVALID_DEVICE_PARTITION_COUNT:\nthe requested device partitioning using CL_DEVICE_PARTITION_BY_COUNTS is not valid.";
            break;
        case CL_INVALID_DEVICE_QUEUE:
            return "CL_INVALID_DEVICE_QUEUE:\nsetting a device queue kernel argument to a value that is not a valid device command-queue.";
            break;
        case CL_INVALID_DEVICE_TYPE:
            return "CL_INVALID_DEVICE_TYPE:\nthe requested device type is not a valid value."; 
            break;
        case CL_INVALID_EVENT:
            return "CL_INVALID_EVENT:\na specified event object is not a valid event object.";
            break;
        case CL_INVALID_GLOBAL_OFFSET:
            return "CL_INVALID_GLOBAL_OFFSET:\nthe specified global offset and global work size exceeds the limits of the device.";
            break;
        case CL_INVALID_GLOBAL_WORK_SIZE:
            return "CL_INVALID_GLOBAL_WORK_SIZE:\nthe specified global work size exceeds the limits of the device.";
            break;
        case CL_INVALID_HOST_PTR:
            return "CL_INVALID_HOST_PTR:the specified host pointer is not valid for the specified flags";
            break;
        case CL_INVALID_IMAGE_DESCRIPTOR:
            return "CL_INVALID_IMAGE_DESCRIPTOR:\nthe specified image descriptor is NULL or specifies invalid values.";
            break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:\nthe specified image format descriptor is NULL or specifies invalid value.";
            break;
        case CL_INVALID_IMAGE_SIZE:
            return "CL_INVALID_IMAGE_SIZE:\nhe specified image dimensions exceed the maximum dimensions for a device or all devices in a context.";
            break;

        case CL_INVALID_KERNEL:
            return "CL_INVALID_KERNEL:\nthe specified kernel is not a valid kernel object.";
            break;
        case CL_INVALID_KERNEL_ARGS:
            return "CL_INVALID_KERNEL_ARGS:\nenqueing a kernel when some kernel arguments have not been set or are invalid.";
            break;
        case CL_INVALID_KERNEL_DEFINITION:
            return "CL_INVALID_KERNEL_DEFINITION:\ncreating a kernel for multiple devices where the number of kernel arguments or kernel argument types are not the same for all devices.";
            break;
        case CL_INVALID_KERNEL_NAME:
            return "CL_INVALID_KERNEL_NAME:\ncreating a kernel when no kernel with the specified name exists in the program object.";
            break;
        case CL_INVALID_LINKER_OPTIONS:
            return "CL_INVALID_LINKER_OPTIONS:\nbuild options passed to clLinkProgram are not valid.";
            break;
        case CL_INVALID_MEM_OBJECT:
            return "CL_INVALID_MEM_OBJECT:\nspecified memory object is not a valid memory object.";
            break;
        case CL_INVALID_OPERATION:
            return "CL_INVALID_OPERATION:\ngeneric error code that is returned when the requested operation is not a valid operation.";
            break;
        case CL_INVALID_PIPE_SIZE:
            return "CL_INVALID_PIPE_SIZE:\nattempting to create a pipe with an invalid packet size or number of packets.";
            break;
        case CL_INVALID_PLATFORM:
            return "CL_INVALID_PLATFORM:\nspecified platform is not a valid platform.";
            break;
        case CL_INVALID_PROGRAM:
            return "CL_INVALID_PROGRAM:\nspecified program is not a valid program object.";
            break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return "CL_INVALID_PROGRAM_EXECUTABLE:\nspecified program is valid but has not been successfully built.";
            break;
        case CL_INVALID_PROPERTY:
            return "CL_INVALID_PROPERTY:\nspecified property name is invalid, when the value for a property name is invalid, or when the same property name is specified more than once.";
            break;
        case CL_INVALID_QUEUE_PROPERTIES:
            return "CL_INVALID_QUEUE_PROPERTIES:\nspecified queue properties are valid but are not supported by the device.";
            break;
        case CL_INVALID_SAMPLER:
            return "CL_INVALID_SAMPLER:\na specified sampler is not a valid sampler object.";
            break;
        case CL_INVALID_SPEC_ID:
            return "CL_INVALID_SPEC_ID:\nthe specified specialization constant ID is not valid for the specified program.";
            break;
        case CL_INVALID_VALUE:
            return "CL_INVALID_VALUE:\na generic error that is returned when a specified value is not a valid value.";
            break;
        case CL_INVALID_WORK_DIMENSION:
            return "CL_INVALID_WORK_DIMENSION:\nspecified clEnqueueNDRangeKernel work dimension is not valid.";
            break;
        case CL_INVALID_WORK_GROUP_SIZE:
            return "CL_INVALID_WORK_GROUP_SIZE:\nspecified clEnqueueNDRangeKerne total work-group size is not valid for the specified kernel or device.";
            break;
        case CL_INVALID_WORK_ITEM_SIZE:
            return "CL_INVALID_WORK_ITEM_SIZE:\nspecified clEnqueueNDRangeKernel work-group size in one dimension is not valid for the device.";
            break;
        case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
            return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE:\nclGetKernelArgInfo kernel argument information is not available for the specified kernel.";
            break;
        case CL_LINK_PROGRAM_FAILURE:
            return "CL_LINK_PROGRAM_FAILURE:\nclLinkProgram failure to link the specified binaries or libraries.";
            break;
        case CL_LINKER_NOT_AVAILABLE:
            return "CL_LINKER_NOT_AVAILABLE:\nclLinkProgram CL_DEVICE_LINKER_AVAILABLE is CL_FALSE.";
            break;
        case CL_MAP_FAILURE:
            return "CL_MAP_FAILURE:\nthere is a failure to map the specified region into the host address space.";
            break;
        case CL_MEM_COPY_OVERLAP:
            return "CL_MEM_COPY_OVERLAP:\ncopying from one region of a memory object to another where the source and destination regions overlap.";
            break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "CL_MEM_OBJECT_ALLOCATION_FAILURE:\nfailure to allocate memory for a memory object.";
            break;
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            return "CL_MISALIGNED_SUB_BUFFER_OFFSET:\na sub-buffer object is created or used that is not aligned to CL_DEVICE_MEM_BASE_ADDR_ALIGN for the device.";
            break;
        case CL_OUT_OF_HOST_MEMORY:
            return "CL_OUT_OF_HOST_MEMORY:\ngeneric error that is returned when memory could not be allocated on the host.";
            break;
        case CL_OUT_OF_RESOURCES:
            return "CL_OUT_OF_RESOURCES\ngeneric error that is returned when resources could not be allocated on the device.";
            break;
        case CL_MAX_SIZE_RESTRICTION_EXCEEDED:
            return "CL_MAX_SIZE_RESTRICTION_EXCEEDED:\nsize of the specified kernel argument value exceeds the maximum size defined for the kernel argument.";
            break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            return "CL_PROFILING_INFO_NOT_AVAILABLE:\nclGetEventProfilingInfo command associated with the specified event was not enqueued into a command-queue with CL_QUEUE_PROFILING_ENABLE.";
            break;

        // Extension errors are here, but commented out for now.
        // I may need them in the future.
        /*
        case CL_INVALID_COMMAND_BUFFER_KHR:
            return "CL_INVALID_COMMAND_BUFFER_KHR:\nspecified command-buffer is not a valid command-buffer.";
            break;
        case CL_INVALID_SYNC_POINT_WAIT_LIST_KHR:
            return "CL_INVALID_SYNC_POINT_WAIT_LIST_KHR:\nspecified sync point wait list or number of sync points in the wait list is not valid.";
            break;
        case CL_INCOMPATIBLE_COMMAND_QUEUE_KHR:
            return "CL_INCOMPATIBLE_COMMAND_QUEUE_KHR:\none or more command-queues is incompatible with a command-buffer.";
            break;
        case CL_INVALID_MUTABLE_COMMAND_KHR:
            return "CL_INVALID_MUTABLE_COMMAND_KHR:\nspecified command is not a valid mutable-command object.";
            break;
        case CL_INVALID_D3D10_DEVICE_KHR:
            return "CL_INVALID_D3D10_DEVICE_KHR:\na Direct3D 10 device cannot interoperate with OpenCL device IDs.";
            break;
        case CL_INVALID_D3D10_RESOURCE_KHR:
            return "CL_INVALID_D3D10_RESOURCE_KHR:\nan OpenCL object cannot be created from a Direct3D 10 resource.";
            break;
        case CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR:
            return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR:\nattempting to acquire an OpenCL object created from a Direct3D 10 resource that was already acquired.";
            break
        case CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR:
            return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR:\nattempting to release an OpenCL object created from a Direct3D 10 resource that has not been acquired.";
            break;
        case CL_INVALID_D3D11_DEVICE_KHR:
            return "CL_INVALID_D3D11_DEVICE_KHR:\na Direct3D 11 device cannot interoperate with OpenCL device IDs.";
            break;
        case CL_INVALID_D3D11_RESOURCE_KHR:
            return "CL_INVALID_D3D11_RESOURCE_KHR:\nan OpenCL object cannot be created from a Direct3D 11 resource.";
            break;
        case CL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR:
            return "CL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR:\nattempting to acquire an OpenCL object created from a Direct3D 11 resource that was already acquired.";
            break;
        case CL_D3D11_RESOURCE_NOT_ACQUIRED_KHR:
            return "CL_D3D11_RESOURCE_NOT_ACQUIRED_KHR:\nattempting to release an OpenCL object created from a Direct3D 11 resource that has not been acquired.";
            break;
        case CL_INVALID_DX9_MEDIA_ADAPTER_KHR:
            return "CL_INVALID_DX9_MEDIA_ADAPTER_KHR:\nDirectX 9 media adapter cannot interoperate with OpenCL device IDs.";
            break;
        case CL_INVALID_DX9_MEDIA_SURFACE_KHR:
            return "CL_INVALID_DX9_MEDIA_SURFACE_KHR:\nDirectX 9 media adapter cannot interoperate with OpenCL device IDs.";
            break;
        case CL_DX9_MEDIA_SURFACE_ALREADY_ACQUIRED_KHR:
            return "CL_DX9_MEDIA_SURFACE_ALREADY_ACQUIRED_KHR:\nattempting to acquire an OpenCL object created from a DirectX 9 media surface that was already acquired.";
            break;
        case CL_DX9_MEDIA_SURFACE_NOT_ACQUIRED_KHR:
            return "CL_DX9_MEDIA_SURFACE_NOT_ACQUIRED_KHR:\nattempting to release an OpenCL object created from a DirectX 9 media surface that has not been acquired.";
            break;
        case CL_EGL_RESOURCE_NOT_ACQUIRED_KHR:
            return "CL_EGL_RESOURCE_NOT_ACQUIRED_KHR:\nEGL resource is used without being acquired.";
            break;
        case CL_INVALID_EGL_OBJECT_KHR:
            return "CL_INVALID_EGL_OBJECT_KHR:\nthe specified EGL object is not valid.";
            break;
        case CL_INVALID_GL_OBJECT:
            return "CL_INVALID_GL_OBJECT:\nthe specified OpenGL object is not valid, or when there is no associated OpenGL object for an OpenCL.";
            break;
        case CL_INVALID_GL_OBJECT:
            return "CL_INVALID_GL_OBJECT:\nhe specified OpenGL object is not valid, or when there is no associated OpenGL object for an OpenCL object.";
            break;
        case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
            return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:\nthe specified OpenGL sharing context creation property is not valid.";
            break;
        case CL_PLATFORM_NOT_FOUND_KHR:
            return "CL_PLATFORM_NOT_FOUND_KHR:\nno platforms are available (clGetPlatformIDs).";
            break;
        case CL_INVALID_SEMAPHORE_KHR:
            return "CL_INVALID_SEMAPHORE_KHR:\nspecified semaphore is not a valid semaphore.";
            break;
        case CL_CONTEXT_TERMINATED_KHR:
            return "CL_CONTEXT_TERMINATED_KHR:\nspecified context has already been terminated, or as an event status for terminated commands.";
            break;
        */
        default: return "Unspecified OpenCL error...";
    }
}
