project(HarmonicMap)

# Find MeshLib
find_package(MeshLib REQUIRED HINTS "${CMAKE_SOURCE_DIR}/3rdparty")
if (MeshLib_FOUND)
  include_directories("${MeshLib_DIR}/core")
else()
  message(FATAL_ERROR "MeshLib Not Found!")
endif()

include_directories("${CMAKE_SOURCE_DIR}/3rdparty/glad/include"
                    "${CMAKE_SOURCE_DIR}/3rdparty/glfw3/include"
                    "${CMAKE_SOURCE_DIR}/3rdparty/"
                    "${CMAKE_SOURCE_DIR}/3rdparty/glm"
                    "${CMAKE_SOURCE_DIR}/3rdparty/stb"
	            "${CMAKE_SOURCE_DIR}/3rdparty/eigen")

file(GLOB SRCS
               "gl/*.h"
               "gl/*.cpp"
               "Viewer.cpp" 
               "GLWindow.h" 
               "GLWindow.cpp" 
               "Arcball.cpp"
               "${CMAKE_SOURCE_DIR}/3rdparty/glad/src/glad.c"
	       "HarmonicMap.h"
               "HarmonicMap.cpp"
               "HarmonicMapMesh.h")

add_executable(HarmonicMap ${SRCS})

# Link the libraries of glfw3
if(MSVC)
    target_link_libraries(HarmonicMap "${CMAKE_SOURCE_DIR}/3rdparty/glfw3/lib-vc2019/glfw3.lib")
endif()
