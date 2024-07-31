# Quibble
An experimental meshless rendering engine that uses a variation on splatting with user-defined function systems.

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

## Past Experiences

I have personally written a number of animation engines before.
Several of them were vector-based (with libraries like Cairo).
One was with OpenGL.
And another couple were with Julia.

I think it is important to highlight my latest attempt at this very engine in Julia: [Fable.jl](https://github.com/leios/Fable.jl).
That project is (was?) very, very similar to this one.
It is an attempt to evaluate user-generated functions at runtime by using Julia tooling.
In a sense, it works.
I have been able to make animations with it.

The problem is that as the complexity of the scene grows, so does the complexity of the final function we are reading in to the GPU.
This means that the compile time jumps up.
For relatively simple functions, the compile time increased to something around 70 seconds on my GPU.
On the other hand, runtime was on the order of 0.0001 seconds for the same kernel.

I tried everything to fix the problem:

1. **Evaluating Exprs (essentially strings) at runtime**: This runs into issues because Julia's compiler wants to compile everything ahead of time, so it will try to compile the encapsulating functions too soon (before user functions are generated). When the user finally does generate their functions, Julia can't reason about them without also either recompiling the encapsulating functions or using `Base.@invokelatest`. These are alright solutions, but ultimately hurt performance and force users to think about the Julia compiler a little too much for my liking.
2. **Macros of all sorts**: The other option provided by Julia is macros. You can roll your own macros to massage use input into a good enough form to then `@generate` the appropriate functions at runtime. Due to a bug (or oversight), these `@generated` functions are absolutely necessary for function iteration ([1](https://github.com/JuliaGPU/CUDA.jl/issues/2450), [2](https://github.com/JuliaGPU/GPUCompiler.jl/issues/607), [3](https://github.com/JuliaGPU/CUDA.jl/issues/2450)).

This took years and required a massive overhaul of my code each time I needed to change anything.
On top of that, because the code was written in Julia, it was quite difficult to make the correct changes.
So I'm rewriting everything in C with OpenCL and Vulkan.

Is this a bad idea?
Maybe.

At this point, I don't really care.
I've sunk so much time into this project that I need something to come out of it.

