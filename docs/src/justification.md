# Design Justifications

There have been multiple design decisions that have been made throughout the development process of this code base that might have been questionable in hindsight.
I will do my best to detail the reasoning behind these decisions here.

To be clear, part of the reason I am making this page is because some of these decisions could be wrong or otherwise based on faulty reasoning.
If you see something that seems weird, please let me know by creating an issue!

## On Languages

Most of the questions I get asked about this project are related to the language of choice -- notably C (and OpenCL for now).
So let's talk about that.

### Why OpenCL?

Simply put, I did a very similar exercise to this one during my PhD and found OpenCL is the best language for development when your users want to actively modify the computational kernels -- specifically because the default option is to compile all kernels at runtime as strings.
This allows me to quickly and easily drop user code into a metaprogrammed kernel in the most straightforward way possible.

Now, it is entirely possible to do runtime compilation in other GPU languages (CUDA, Metal, etc), but OpenCL is a great target to start with before moving to other APIs.

### Why not use Vulkan?

... Or any other graphics library like OpenGL or Metal?
In principle, Vulkan (and OpenGL) also allow for runtime compilation of kernels, however, this project aims to study a fundamentally new take on rendering that does not require meshes at all.
In such a case, there's no reason to use a traditional graphics API when all I need are fast compute shaders / kernels when I can simply pass the buffer over to the appropriate graphics API when the computation is done..

### Why not Julia?

I am not trying to pick a fight here.
I like Julia.
I genuinely believe it's got the best GPGPU ecosystem in the business.
Yet, I tried to get this project to work in Julia for the better part of 4 years and failed after several attempts.
I'm going back to C, where I know it will work and I don't need to think deeply about the language.

This is also my answer to questions on other GPU languages like rust, futhark, and odin.
Yes, these languages could probably be used, but I am not willing to gamble on a language right now.
It's best to just go back to a language I *know* will work without much thought.

## Miscellaneous

Here are a bunch of miscellaneous design decisions I have made.

### Why not OpenCL images?

[OpenCL provides in-built image types](https://registry.khronos.org/OpenCL/specs/3.0-unified/html/OpenCL_C.html#image-read-and-write-functions), like `image2d_t`, which seems to be ideal for this use-case; however, there are some core limitations.
Namely, these images are stored in GPU texture memory, which is the same as global memory, but with a read-only cache associated with it.
This means that it's faster to read, but the same speed to write.
The problem here is that I don't really need fast read speeds, so using texture memory is basically unnecessary overhead that could cause register spilling for no clear advantage.

There are some use-cases, like when dealing with user-provided images, that it might be nice to use a sampler, but they are not useful for the majority of quibble applications.
