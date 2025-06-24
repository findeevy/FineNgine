# FineNgine
A Vulkan and C++ based modern real time GPU based 3D rendering engine that I am building to better understand real-time rendering, Vulkan, and low-level programming.

## Compilation:
Run the makefile after installing the [dependencies](#dependencies). Also make sure to compile the shaders using the provided script [compileShaders.sh](https://github.com/findeevy/Finengine/blob/main/compileShaders.sh). Must have GCC and Linux for compiling, the tools provided probably won't work on macOS or Windows out of the box.

## Features:
- Fragment/vertex shader support.
- Window resizing.
- Texture mapping.
- Depth buffer (Z-depth).
- Discrete graphics device selection.
- GPU/CPU memory management (staging buffer).
- Swap chain and frame buffer management.
- CPU/GPU synchronization.
- Error checking.

## Sample Output:
![Model of Chicken](3dmodel.gif)\
![Rotating Grass Tiles](zbuffer.gif)

## Dependencies:
- [Vulkan](https://www.vulkan.org)
- [GLM](https://github.com/g-truc/glm)
- [STB](https://github.com/nothings/stb)
- [GLFW](https://www.glfw.org)
- [TinyObjLoader](https://github.com/tinyobjloader/tinyobjloader)
<<<<<<< HEAD

=======
>>>>>>> 743ce5c8ed457463656e27f2269e82029f1b9a6e
