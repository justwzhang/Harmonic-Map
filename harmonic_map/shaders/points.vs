#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec4 VertexColor;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform bool u_hasVertexColor = false;
uniform vec3 u_vertexColor = vec3(0.95, 0.95, 0.95);

void main()
{
    if (u_hasVertexColor)
        VertexColor = vec4(inColor, 1.0f);
    else
        VertexColor = vec4(u_vertexColor, 1.0f);
    
    // screen space coordinates of the vertex
    gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(inPos, 1.0f);
}