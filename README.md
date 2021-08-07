# Raven Engine
Raven Engine is a personal game engine, created with the focus of make a game like Diablo(Action RPG, Hack and Slash). So all the features will focus in that kind of games.

## Third Party
All the dependencies are installed using [Conan](https://conan.io/)

| Name | Version |
---|---
| [spdlog](https://github.com/gabime/spdlog) | 1.8.2 | 
| [GLFW](https://github.com/glfw/glfw) | 3.3.4 | 

## Build
### Requirements
* CMake 3.18
* Python 3.9.6
* Conan 1.38.0

#### Conan
For install and config conan there is the [Getting Started](https://docs.conan.io/en/latest/getting_started.html).

### Linux
* GCC 11.1.0
```commandline
mkdir build
cd build
conan install ..
cmake ..
make
```