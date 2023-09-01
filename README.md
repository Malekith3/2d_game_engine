# <span style="color:#e0e0ff">2D Game Engine</span>

---
## <span style="color:#e0e0ff">General Information</span>
I built this game engine for educational purposes, and it is based on the Pikuma  [course](https://pikuma.com/courses/cpp-2d-game-engine-development).

## <span style="color:#e0e0ff">Overview</span>
- This 2D game engine employs the ECS (Entity-Component-System) design philosophy under the hood.
- To streamline the project's build process, I've integrated vcpkg and CMake.
- This project uses the SDL (Simple DirectMedia Layer) library for rendering graphics.
- Additionally, it incorporates the Lua programming language for scripting, seamlessly integrated with C++ using Sol.

## <span style="color:#e0e0ff">Platform Compatibility</span>
The current version of the game engine has been tested and built on macOS and Linux. While it hasn't been tested on Windows yet, the vcpkg integration is expected to simplify the build process for Windows users as well.

# Project Build Guide 

This guide explains how to build the project on different platforms using CMake and vcpkg. Ensure you have the required prerequisites before proceeding.

## <span style="color:#e0e0ff">Prerequisites</span>

- **CMake**: A cross-platform build system. Install it if not already present.

- **vcpkg**: A package manager for C and C++ libraries. You can find installation instructions on the [vcpkg GitHub repository](https://github.com/microsoft/vcpkg).

- **Compiler**: You'll need a C++ compiler compatible with your platform.

## VCPKG Configuration
**VCPKG_ROOT:**
#### Windows
````Bash
setx VCPKG_ROOT "C:\path\to\vcpkg"
````
#### Linux/Macos
````Bash
export VCPKG_ROOT=/path/to/vcpkg
````

========================================================================================================================================================================
## Building the Project  <span style="color:#e0e0ff">Building the Project/span>

**Clone the Repository**: Begin by cloning your project repository.

```bash
   git clone <repository_url>
   cd <project_directory>
```

````bash
vcpkg install --triplet x64-linux # For Linux 
````
````bash
vcpkg install --triplet x64-osx   # For macOS
````
````bash
vcpkg install --triplet x64-windows # For 64-bit Windows
````
**Configure CMake:** Create a build directory and configure the project with CMake.
````bash
mkdir build_folder
cmake -B build_folder -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
````
**Build the Project:**:Compile your project using the generated build files.
````bash
cmake --build build_folder
````

**Executable name:** <span style="color:RED">2d_game_engine</span>
