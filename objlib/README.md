# Object Library
Welcome to the object library team.

## Build Instructions
### Linux
```bash
    cd build
    cmake ..
    make objlib
```

## Project Structure
### CMakeLists.txt
This is our build config for our project. 
CMake is an excellent build system that sets up your programming environment for whatever OS and IDE you plan to use.
* [CMake 3.7 Documentation](https://cmake.org/cmake/help/v3.7/)
* [CMake Wiki](https://cmake.org/Wiki/CMake)
* [CMakeLists.txt Tutorial](https://cmake.org/cmake-tutorial/)
* [How To Build A CMake Project](http://preshing.com/20170511/how-to-build-a-cmake-based-project/)
 
### CMakeLists.txt.in
This is another build script that pulls googletest into our project.
googletest is a solid c/c++ testing environment.
* [googletest Doc's](https://github.com/google/googletest/tree/master/googletest/docs)
* [googletest Getting Started](https://github.com/google/googletest/blob/master/googletest/docs/Primer.md)
* [googletest Usage Example](http://www.yolinux.com/TUTORIALS/Cpp-GoogleTest.html)

### /build
The build directory where CMake is run. This will setup your programming environment (Visual Studio, Eclipse, QT Creator, make).
**Make sure the .gitignore file is always present** otherwise your programming environment will be logged by git!
```bash
    cd build
    cmake ..
```
### /src
All source files(.c .cpp) reside in here.

### /include
All project header files (.h) go here. 

### /test
All project tests go here

### /examples
A directory that holds examples of our project tools.
* googletest dummy tests

## UnQLite Database Engine
A Key/Value store that is serverless, zero configuration, and self-contained without dependency.
* [Documentation](https://unqlite.org/c_api.html)
* [UnQLite In 5 Minutes Or Less](https://unqlite.org/intro.html)
* [Introduction To The UnQLite C/C++ Interfaces](https://unqlite.org/api_intro.html)

## dirent.h
A c/c++ library for retrieving information on files and directories.
* [Linux Documentation](http://pubs.opengroup.org/onlinepubs/009695399/basedefs/dirent.h.html)
* [Win Documentation](https://github.com/tronkko/dirent)

## Test googletest
This is an exercise to get acquainted with cmake and googletests.
I am using Xubuntu 16.04 and make.
```bash
    cd build
    cmake ..
    make WorldTest
    ./bin/WorldTest
```

### Information on proper testing technique
* [Unit Testing - Best Practices](http://www.softwaretestingstuff.com/2010/09/unit-testing-best-practices-techniques.html)
* [An Effective Testing Strategy](https://lostechies.com/jimmybogard/2010/08/25/an-effective-testing-strategy/)


