#version 110
uniform mat4 MVP;
uniform vec3 vCol;
uniform vec3 vPos;
varying vec3 color;
void main()
{
  gl_Position = MVP * vec4(vPos, 1.0);
  color = vCol;
}