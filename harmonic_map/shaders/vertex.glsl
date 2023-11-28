#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
//layout (location = 3) in vec3 inColor;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
uniform int u_showUV;
uniform vec3 u_vertexColor = vec3(0.95, 0.95, 0.95);
//uniform bool u_hasVertexColor = false;

out block
{
    vec4 VertexColor;
    vec2 TexCoord;
    vec3 vNormal;
    vec3 WorldPos;
} Out;

void main()
{
    // position in world space
    Out.WorldPos = vec3(u_modelMat * vec4(inPos, 1.0f));
    if(u_showUV == 1)
        Out.WorldPos = vec3(u_modelMat * vec4(inTexCoord, 0.0f, 1.0f));

    Out.vNormal = mat3(transpose(inverse(u_viewMat * u_modelMat))) * inNormal;
    
    Out.VertexColor = vec4(u_vertexColor, 1.0);
    Out.TexCoord = inTexCoord;

    // screen space coordinates of the vertex
    gl_Position = u_projMat * u_viewMat * vec4(Out.WorldPos, 1.0);
}