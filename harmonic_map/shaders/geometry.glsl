#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in block
{
    vec4 VertexColor;
    vec2 TexCoord;
    vec3 vNormal;
    vec3 WorldPos;
} In[];

out block
{
    vec4 VertexColor;
    vec2 TexCoord;
    vec3 vNormal;
    vec3 fNormal;
    vec3 WorldPos;
} Out;


void main( void )
{
    vec3 a = ( gl_in[1].gl_Position - gl_in[0].gl_Position ).xyz;
    vec3 b = ( gl_in[2].gl_Position - gl_in[0].gl_Position ).xyz;
    vec3 N = normalize( cross( b, a ) );

    for( int i = 0; i < gl_in.length( ); ++i )
    {
        gl_Position = gl_in[i].gl_Position;

        Out.fNormal = N;
        Out.vNormal = In[i].vNormal;
        Out.VertexColor = In[i].VertexColor;
        Out.TexCoord = In[i].TexCoord;
        Out.WorldPos = In[i].WorldPos;

        EmitVertex( );
    }

    EndPrimitive( );
}
