# FineNgine
Vulkan and C++ based modern real time GPU based 3D rendering engine that I am building to better understand real-time rendering, Vulkan, and low-level programming.

## Sample Output:
![Rotating Grass Tiles](zbuffer.gif)

## Compilation:
Run the makefile after installing the dependencies (listed below). Also make sure to compile the shaders using the provided script (compileShaders.sh). Must have GCC (or other C++ compiler, although I have not tested any others).

## Features:
-Fragment/vertex shader support.\
-Window resizing.\
-Texture mapping.\
-Depth buffer (Z-depth).\
-Discrete graphics device selection.\
-GPU/CPU memory management (staging buffer).\
-Swap chain and frame buffer management.\
-CPU/GPU synchronization.\
-Error checking.

## Dependencies:
- [Vulkan](https://www.vulkan.org)
- [GLM](https://github.com/g-truc/glm)
- [STB](https://github.com/nothings/stb)
- [GLFW](https://www.glfw.org)
