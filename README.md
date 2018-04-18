# Project info
This was the project done in CS485. I was the creator of the backend for this project which can be found in the objlib folder.


# Project WebMind

This is the repository for Project WebMind.

## Build Dependencies

In addition to standard C++ and Java build-tools, the following tools/libraries
are required:

* [CMake](https://cmake.org/) (>= 3.0)
* [Git](https://git-scm.com/)

## Build Instructions

To build WebMind ensure that all build dependencies are installed and configured
properly. Then, issue the following commands to generate build files:

    $ mkdir build
    $ cd build
    $ cmake ..
    
If you'd like to build unit tests, execute the following CMake command instead:

    $ cmake -DBUILD_TESTS=ON ..
    
After the build files for your build environment have been created, execute the
correct command for your build chain. On linux and macOS, this will likely be:

    $ make
    
On Windows it will likely be MSBuild by default. Note that the
`Configuration=Debug` parameter is set to ensure that debugging symbols are
built into the binary outputs. To disable this change the parameter to
`Configuration=Release`. To build on Windows with MSBuild issue the following
command:

    $ msbuild WebMind.sln /p:Configuration=Debug
    
For additional information on using CMake as a buildchain generator, see
the [CMake documentation](https://cmake.org/cmake/help/v3.0/).

## GUI Runtime Commands

### At Launch
Enter a new world name and click "Create".

### At Runtime
All implemented functions can be run from the Menu Bar at the top of the 
application.

Pointer Tool is set as the default mouse state.

### Grid Edits
#### Pointer Tool
Click cells to flip on or off.

#### Selection Tool
Left Click and Drag a region on Grid.  
Right Click to flip entire region.  

#### Simulation Speed Settings  
1 second  
500 ms  
250 ms  
125 ms  
33 ms (30 fps)  

### Keybindings
Arrow Keys - Move Grid (UP, DOWN, LEFT, RIGHT)  
Page Up/Page Down - Zoom in/out  
Mouse Wheel Up/Down - Zoom in/out  
Enter - Step to next frame  
Space - Play/Pause  
[1] - 1 second speed setting  
[2] - 500 ms speed setting  
[3] - 250 ms speed setting  
[4] - 125 ms speed setting  
[5] - 33 ms speed setting  
  
NOTE: The frame cannot be editted during PLAY mode. Pause the sim to resume edits.
