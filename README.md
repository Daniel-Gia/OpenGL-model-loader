# OpenGL Model Loader

[![Format](https://github.com/Daniel-Gia/OpenGL-model-loader/actions/workflows/ci.yml/badge.svg?job=format)](https://github.com/Daniel-Gia/OpenGL-model-loader/actions/workflows/ci.yml)
[![Lint](https://github.com/Daniel-Gia/OpenGL-model-loader/actions/workflows/ci.yml/badge.svg?job=lint)](https://github.com/Daniel-Gia/OpenGL-model-loader/actions/workflows/ci.yml)
[![Test Build](https://github.com/Daniel-Gia/OpenGL-model-loader/actions/workflows/ci.yml/badge.svg?job=build)](https://github.com/Daniel-Gia/OpenGL-model-loader/actions/workflows/ci.yml)

A Windows only **OpenGL model loader** written in C++. 
The project uses GLFW, GLAD, GLM, stb, and fastgltf, with CMake and vcpkg handling the build and dependencies.

## Demo
[Demo](https://github.com/user-attachments/assets/6ad7a95b-fc89-4454-bb4f-96ead02c29fc)

## Requirements
Install these tools before building:

- Windows 10 or later
- Git
- CMake 3.20 or newer
- MinGW GCC
- LLVM, including `clang-format` and `clang-tidy`
- PowerShell

If you use Chocolatey, install the required tools with:

```powershell
choco install git cmake mingw llvm --yes
```

Restart your terminal after installing the tools so the updated `PATH` is available.

## Setup

Clone the repository:

```powershell
git clone https://github.com/Daniel-Gia/OpenGL-model-loader.git
```

Set up vcpkg:

```powershell
.\scripts\Setup-vcpkg.ps1
```

This script clones vcpkg, checks out the project’s pinned version, and bootstraps it. The dependencies in `vcpkg.json` are installed automatically by CMake during configuration.

## Build And Run

Configure the Debug build:

```powershell
.\scripts\Configure.ps1
```

Build the project:

```powershell
.\scripts\Build.ps1
```

Run the application:

```powershell
.\scripts\Run.ps1
```

To build and run in one step:

```powershell
.\scripts\BuildAndRun.ps1
```

The Debug executable is generated at:

```text
build\debug\OpenGL-Model-Loader.exe
```

## Release Build
```powershell
.\scripts\Release.ps1
```

The final release build is inside:
```text
final_build\
```

## Code Quality

Run the formatting check:
```powershell
.\scripts\CheckFormat.ps1
```

Run the clang-tidy check:
```powershell
.\scripts\CheckLint.ps1
```

### Apply Automatic Fixes

Format:
```powershell
.\scripts\Format.ps1
```

Lint:
```powershell
.\scripts\FixLint.ps1
```

## Cleaning Build Files
```powershell
.\scripts\Clean.ps1
```
