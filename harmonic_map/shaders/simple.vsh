#version 330

// attributes
layout(location = 0) in vec3 i_position;	// xyz - position
layout(location = 1) in vec2 i_texcoord;	// uv  - texture coords
layout(location = 2) in vec3 i_normal;		// xyz - normal

//out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(i_position, 1.0f);
    //ourColor = color;
    TexCoord = i_texcoord;
}