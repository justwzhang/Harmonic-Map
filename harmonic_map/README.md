# Harmonic Map

This C++ project framework is used to help students to implement harmonic map algorithm. It contains a simple opengl viewer.

## Dependencies
 
1. `MeshLib`, a mesh library based on halfedge data structure.
2. `Eigen`, a C++ template library for linear algebra.
3. `glfw`, a multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop.
4. `glm`, a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
5. `glad` includes file for GLAD includes the required OpenGL headers behind the scenes (like GL/gl.h).

## Directory Structure

``` txt
CMakeLists.txt   -- CMake configuration file.
```

## Configuration

### Windows

1. Install [CMake](https://cmake.org/download/).

2. Download the source code of the C++ framework.
> E.x. I create a folder `projects` in `C:/`, then unzip the source code there.

3. Configure and generate the project for Visual Studio.

> ``` bash
> cd CCGHomework
> mkdir build
> cd build
> cmake ..
> ```
> *One can also finish this step using CMake GUI.*

4. Open the \*.sln using Visual Studio, and complie the solution.

5. Finish your code in your IDE.

6. Run the executable program.
> E.x. 
> ``` bash
> cd bin
> ./HarmonicMap.exe ../data/boy.m
> ```

7. Press '?' when your mouse is focused on the glut window, and follow the instruction in the command line window.
> If you can see the following results, then it means that you have finished the harmonic map algorithm. 
> 
> ![A face](../resources/boy.png) ![Harmonic map](../resources/boy_harmonic_map.png)

### Linux & Mac

1. Build and compile the code.

> ``` bash
> cd CCGHomework
> mkdir build
> cd build
> cmake ..
> make && make install
> ```

2. Run the executable program.

> ``` bash
> cd ../bin/
> ./HarmonicMap ../data/boy.m
> ```
