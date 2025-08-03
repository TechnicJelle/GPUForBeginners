# How To Get Started With GPU Programming for Beginners

If you want to start GPU Programming but don't know how yet, you have come to the right place!
This online book will guide you through the whole process!

From your first blank window, to your first triangle, to a 3D model loader, and hopefully beyond!

The code of this guide will be C++, because that is by far the most common language in the Graphics Programming industry today.
So this guide assumes that you are already familiar with C++.

## A quick history lesson on GPU Programming

There exist many APIs with which you can control GPUs.
Many of them are old and antiquated,
and many of them only work on certain platforms, OSes, devices, and consoles.

Luckily, there is a very nice library that wraps all these various platform APIs: SDL3.

It works on essentially every device and platform, notably all modern computers and game consoles.  
Some mobile devices are also supported, and web support is planned as well.

SDL3's GPU API is heavily inspired by the famously verbose Vulkan, but it is much more condensed,
so you should be able to get up and running relatively quickly.

It contains essentially all features than a normal 3D application or game could need,
but if you need very platform-specific features or very modern functionality, you should look at other APIs instead.  
But even then, SDL's GPU API is likely a good introduction for those more complicated APIs.

So without further ado, let's get started, shall we!

## Index

1. [Chapter 0: Setup](chapter00/README.md)
2. [Chapter 1: Blank Screen](chapter01/README.md)
3. [Chapter 2: First Triangle](chapter02/README.md)

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
