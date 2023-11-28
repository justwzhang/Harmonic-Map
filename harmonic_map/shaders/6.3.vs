#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  


layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 1) in vec3 aNormal;

out vec2 oTexCoord;
out vec4 oFragColor; // 输出的混合光

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_pos;     /* world coordinate */
uniform vec3 camera_pos;    
uniform vec3 object_pos;    

//uniform vec3 light_dir;     // 光源方向 只是一个方向
uniform vec3 light_color;   // 环境光源颜色
//uniform vec3 light_diffuse; // 漫反射 模拟材质补光用
uniform Material material;

void main()
{
    /* vNormal = normalize(vec3(model * vec4(aNormal, 0.0))); */
    
    // vec2(aTexCoord.x, 1.0 - aTexCoord.y);

    /* Normals in camera space */
    // vNormal = (view * model * vec4( aNormal, 0.0 )).xyz;


    oTexCoord = aTexCoord;

    // ambient
    vec3 ambient = vec3(0.2) * light_color * material.ambient;

    // diffuse 
    vec3  norm     = normalize(aNormal);
    vec3  lightDir = normalize(light_pos - object_pos);
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = vec3(0.8) * light_color * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(camera_pos - object_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(0.8) * light_color * (spec * material.specular);

    vec4 v_color = vec4(229.0 / 255.0f, 162.0 / 255.0f, 141.0 / 255.0f, 1.0f);

    vec3 result = ambient + diffuse + specular;
    oFragColor = vec4(result, 1.0) * 0.75  + v_color * 0.6;

    //float lightStrength   =   max(dot(aNormal, -light_dir), 0.0);
    //oComposeColor = vec4(light_color * lightStrength + light_diffuse, 1.0f);

    
    gl_Position   = projection * view * model * vec4(aPos, 1.0f);
}
