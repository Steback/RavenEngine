# Raven Engine
Raven Engine is a personal game engine, created with the focus of make a game like Diablo(Action RPG, Hack and Slash). So all the features will focus in that kind of games.

## Third Party
The dependencies are installed using [Conan](https://conan.io/)

| Name | Version |
---|---
| [spdlog](https://github.com/gabime/spdlog) | 1.8.2 | 
| [GLFW](https://github.com/glfw/glfw) | 3.3.4 | 
| [nlohmann_json](https://github.com/nlohmann/json) | 3.9.1 | 
| [Vulkan Headers](https://github.com/KhronosGroup/Vulkan-Headers) | 1.2.182 | 
| [Vulkan Loader](https://github.com/KhronosGroup/Vulkan-Loader) | 1.2.182 | 
| [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) | 2.3.0 | 
| [Catch2](https://github.com/catchorg/Catch2) | 2.13.7 | 
| [TinyGLTF](https://github.com/syoyo/tinygltf) | 2.5.0 | 
| [Entt](https://github.com/skypjack/entt) | 3.8.1 | 
| [ImGuiRender](https://github.com/ocornut/imgui) | 1.84.1 | 
| [Shaderc](https://github.com/google/shaderc) | 2021.1 | 

## Build
### Requirements
* CMake 3.18 or higher
* Python 3.x
* Conan 1.38.0 or higher

#### Conan
For install and config conan there is the [Getting Started](https://docs.conan.io/en/latest/getting_started.html).

### Linux
* GCC 11.1.0 
```commandline
mkdir build && cd build
conan install ..
cmake ..
make
```

### Windows
* Visual Studio 2019
```commandline
mkdir build && cd build
conan install ..
cmake ..
cmake --build . --config Release 
```

## Samples
All the examples are includes in the [samples](samples) folder.

## Documentation
The documentation are generated using [Doxygen](https://www.doxygen.nl/index.html) with the [Doxyfile](Doxyfile);