#version 330

// attributes
layout(location = 0) in vec3 i_position;	// xyz - position
layout(location = 1) in vec2 i_texcoord;	// uv  - texture coords
layout(location = 2) in vec3 i_normal;		// xyz - normal

// matrices
uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
//uniform mat3 u_normalMat;

// position of light and camera
uniform vec3 u_lightPosition;
uniform vec3 u_cameraPosition;

uniform int u_showUV;

// data for fragment shader
out vec3 o_normal;
out vec3 o_toLight;
out vec3 o_toLight2;
out vec3 o_toCamera;
out vec2 o_texcoords;

///////////////////////////////////////////////////////////////////

void main(void)
{
   // position in world space
   vec4 worldPosition = u_modelMat * vec4(i_position, 1.0f);
   if(u_showUV == 1)
		worldPosition = u_modelMat * vec4(i_texcoord, 1.0f, 1.0f);

   // normal in world space
   //o_normal = normalize(u_normalMat * i_normal);
   o_normal = normalize(i_normal);

   // direction to light
   o_toLight = normalize(u_lightPosition - worldPosition.xyz);
   o_toLight2= normalize(-u_lightPosition - worldPosition.xyz);

   // direction to camera
   o_toCamera = normalize(u_cameraPosition - worldPosition.xyz);

   // texture coordinates to fragment shader
   o_texcoords = i_texcoord;

   // screen space coordinates of the vertex
   gl_Position = u_projMat * u_viewMat * worldPosition;
}
