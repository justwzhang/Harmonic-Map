#version 330 core

struct DirLight {
    vec3  position;
    vec3  color;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
};

struct Material {
    //sampler2D diffuse;
    //sampler2D specular;
    vec3 ambient;   // Ambient Reflectances
    vec3 diffuse;   // Diffuse Reflectances
    vec3 specular;  // Specular Reflectances
    float shininess;
}; 

struct Texture {
    sampler2D rgb;
    int mode;   // 0: NONE, 1: Replace, 2: Modulate
};

in block
{
    vec4 VertexColor;
    vec2 TexCoord;
    vec3 vNormal;
    vec3 fNormal;
    vec3 WorldPos;
} In;

out vec4 FragColor;

#define NUM_DIR_LIGHTS 2

uniform Texture u_texture;
uniform vec3 u_viewPos; 
uniform DirLight u_dirLight[NUM_DIR_LIGHTS];
uniform Material u_material;
uniform int u_DrawMode = 3;   

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 normal = In.vNormal;
    if (u_DrawMode == 3)
        normal = In.fNormal;

    vec3 viewDir = normalize(u_viewPos - In.WorldPos);
    
    vec3 shaderness = vec3(0, 0, 0);
    for(int i = 0; i < NUM_DIR_LIGHTS; i++)
        shaderness += 0.5 * CalcDirLight(u_dirLight[i], normal, viewDir);

    if (u_texture.mode == 1)
        FragColor = texture(u_texture.rgb, In.TexCoord) * vec4(shaderness, 1.0);
    else if (u_texture.mode == 2)
        FragColor = texture(u_texture.rgb, In.TexCoord) * vec4(shaderness, 1.0) * In.VertexColor;
    else if (u_texture.mode == 3)
       FragColor = vec4(normal, 1.0);
    else if (u_texture.mode == 4)
       FragColor = mix(texture(u_texture.rgb, In.TexCoord), In.VertexColor, 0.8) * vec4(shaderness, 1.0);
        //FragColor = texture(u_texture.rgb, In.TexCoord) * In.VertexColor;
    else
        FragColor = vec4(shaderness, 1.0) * In.VertexColor;
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - In.WorldPos);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * light.color * u_material.ambient;           //* vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * light.color * diff  * u_material.diffuse;    //* diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * light.color * spec  * u_material.specular;  //* spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular);
}
