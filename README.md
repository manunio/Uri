# Uri

This is a library which implements [RFC 3986](https://tools.ietf.org/html/rfc3986),
"Uniform Resource Identifier (URI): Generic Syntax".

## Usage

The `Uri::Uri` class is used to parse URIs from strings, render URIs as strings, and get or set the individual elements of a URI.

## Supported platforms / recommended toolchains

This is a portable C++11 library which depends only on the C++11 compiler and standard library, so it should be supported on almost any platform.  The following are recommended toolchains for popular platforms.

* Windows -- [Visual Studio](https://www.visualstudio.com/) (Microsoft Visual C++)
* Linux -- clang or gcc
* MacOS -- Xcode (clang)

## Building

This library is not intended to stand alone.  It is intended to be included in a larger solution which uses [CMake](https://cmake.org/) to generate the build system and build applications which will link with the library.

There are two distinct steps in the build process:

1. Generation of the build system, using CMake
2. Compiling, linking, etc., using CMake-compatible toolchain

### Prerequisites

* [CMake](https://cmake.org/) version 3.8 or newer
* C++11 toolchain compatible with CMake for your development platform (e.g. [Visual Studio](https://www.visualstudio.com/) on Windows)

### Build system generation

Generate the build system using [CMake](https://cmake.org/) from the solution root.  For example:
* Windows 
```bash
mkdir build
cd build
cmake -G "Visual Studio 15 2017" -A "x64" ..
```
* Linux
```shell script
mkdir build
cd build
cmake ..
```
### Compiling, linking, et cetera

Either use [CMake](https://cmake.org/) or your toolchain's IDE to build.
For [CMake](https://cmake.org/):
* Windows
```bash
cd build
cmake --build . --config Release
```
* Linux
```shell script
cd build
make
```