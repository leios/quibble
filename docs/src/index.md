# What is Quibble?
Quibble is an experimental meshless rendering engine that uses a variation on splatting with user-defined function systems.

This project has a few overarching goals:

1. **Research**: I am genuinely interested to see how far we can push meshless (and in principle memoryless) rendering.
2. **Performance**: I would like to guarantee that users can easily manipulate their function systems and see the results in (close to) realtime. For this to happen, we need to focus on both runtime performance *and* compilation performance.
3. **Usability**: I would like to use this engine to actually animate high quality videos to put out on YouTube (and also promote related written material).

Because this project does not use the traditional mesh machinery that is common in most graphical pipelines, we do not need to focus on shaders, but instead on compute kernels that are more common in CUDA, OpenCL, or other GPGPU languages (like Julia's KernelAbstractions.jl).
It is also possible to use Vulkan's compute shaders for the same purpose and it might be interesting to consider doing that in the future, especially if OpenCL JIT compilation ends up becoming a problem.
In that case, it could be possible to spin up our own SPIRV compiler to send to Vulkan for the "fastest compilation performance possible."

## Immediate plan

Right now, the project is in its infancy.
My plan is to do the following:

1. Implement simple, user-defined function systems that are compiled at runtime in the form of an OpenCL kernel.
2. Write the kernel results to file (either in the form of png images or video directly).
2. Use the engine in a real animation for my YouTube channel.

After that, I have a few nebulous goals.
For example if I want:
1. Any sort of real time output: I might need to blit the OpenCL result to Vulkan.
2. A game: I might also require some form of audio library
3. A modeling UI (like blender, maybe): I would need to reason about how to take user input and translate that into a function system. I think this could be done with simple mouse gestures, but maybe ML as well?
4. A minimal set of points for each frame: to do this automatically, I would need to use some form of auto differentiation, but I think my generators should be able to take in a user-defined derivative from the start. 

