# How To Get Started With GPU Programming for Beginners

If you want to start GPU Programming or Graphics Programming but don't know how yet,
you have come to the right place!  
This online book will guide you through the whole process!

From your first blank window, to your first triangle, to a 3D model loader, and hopefully beyond!

## Why and what?

There exist many APIs with which you can control GPUs.
Many of them are old and antiquated,
and many of them only work on certain platforms, OSes, devices, and consoles.

Luckily, there is a very nice library that wraps all these various platform APIs: [SDL3](https://www.libsdl.org/).

It works on essentially every device and platform, notably all modern computers and game consoles.  
Some mobile devices are also supported, and web support is planned as well.

SDL3's GPU API is heavily inspired by the famously verbose Vulkan, but it is much more condensed,
so you should be able to get up and running relatively quickly.

It contains essentially all features that a normal 3D application or game could need,
but if you need very platform-specific features or very modern functionality, you should look at other APIs instead.  
But even then, SDL's GPU API is a good introduction for those more complicated platform APIs.

The code of this guide will be C++, because that is by far the most common language
in the Graphics Programming industry today. So this guide assumes that you are already familiar with C++.

That said, you can follow along using different languages, because the main focus here is SDL3's GPU API.  
You can translate the C++ concepts that are used to other languages while reading.  
And SDL3 itself is available in many other languages as well, through third-party bindings.

So without further ado, let's get started, shall we!

## Index

1. [Chapter 1: Setup](chapter01/README.md)  
   (Development Environment, Project Creation, Game Loop, Window)
2. [Chapter 2: Blank Screen](chapter02/README.md)  
   (GPU Device, Swapchain, Command Buffers, Render Passes)
3. [Chapter 3: First Triangle](chapter03/README.md)  
   (Vertices, Pipelines, Vertex Buffers, Backface Culling)
4. [Chapter 4: Rainbow Triangle](chapter04/README.md)  
   (Additional per-vertex data, Packed data, Vertex Colors)
5. [Chapter 5: Quad](chapter05/README.md)  
   (Index Buffers)
6. [Chapter 6: Textured Quad](chapter06/README.md)  
   (Textures, Samplers)
7. [Chapter 7: Perspective](chapter07/README.md)  
   (Uniform Buffers, Maths Libraries, 3D)
8. [Chapter 8: 3D Model Loading](chapter08/README.md)  
   (Model loader libraries, Properly storing mesh data)
9. [Chapter 9: Depth](chapter09/README.md)  
   (Depth Buffers, Window Resizing)

## Support

If, at any time, you get stuck following this guide, or if you have a question,
please don't hesitate to ask in one of the [SDL Communities](https://wiki.libsdl.org/SDL3/FAQCommunities)!

## Other Info

The source code for this website, and the full source code for all the chapters is hosted on GitHub:
[github.com/TechnicJelle/GPUForBeginners](https://github.com/TechnicJelle/GPUForBeginners)

Contributions are ***very*** welcome! However small they may be!

## Acknowledgements

Firstly, I would like to thank the SDL developers and contributors for making such an amazing library!

I would also like to thank in particular the
[SDL GPU Examples repository](https://github.com/TheSpydog/SDL_gpu_examples),
which I used a lot while learning SDL's GPU API myself,
and also while writing this guide. Much code of this guide originally comes from there, so check it out!

I would also like to thank Joey de Vries for making [LearnOpenGL.com](https://learnopengl.com/)
which was a huge inspiration for this guide.
