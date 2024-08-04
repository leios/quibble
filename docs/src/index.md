# What is Quibble?
Quibble is an experimental meshless rendering engine that uses a variation on splatting with user-defined function systems.

This project has a few overarching goals:

1. **Research**: I am genuinely interested to see how far we can push meshless (and in principle memoryless) rendering.
2. **Performance**: I would like to guarantee that users can easily manipulate their function systems and see the results in (close to) realtime. For this to happen, we need to focus on both runtime performance *and* compilation performance.
3. **Usability**: I would like to use this engine to actually animate high quality videos to put out on YouTube (and also promote related written material).

Because this project does not use the traditional mesh machinery that is common in most graphical pipelines, we do not need to focus on shaders, but instead on compute kernels that are more common in CUDA, OpenCL, or other GPGPU languages (like Julia's KernelAbstractions.jl).
It is also possible to use Vulkan's compute shaders for the same purpose and it might be interesting to consider doing that in the future, especially if OpenCL JIT compilation ends up becoming a problem.
In that case, it could be possible to spin up our own SPIRV compiler to send to Vulkan for the "fastest performance possible."

## Current Goals

Right now, the project is in its infancy.
My plan is to do the following:

1. Implement simple, user-defined function systems that are compiled at runtime in the form of an OpenCL kernel.
2. Blit the results of the OpenCL kernel to Vulkan for realtime rendering and a simple UI.
3. Use the engine in a real animation for my YouTube channel.
