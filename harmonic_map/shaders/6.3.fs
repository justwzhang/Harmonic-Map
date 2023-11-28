#version 330 core

in vec4 oFragColor;
in vec2 oTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
    // Uncomment these lines to use the fragment's original color
    // vec4 color = texture(_texture, oTexCoord);
    // FragColor = color * oFragColor;
    
    float blendFactor = 0.5; 
    
    vec4 blendedColor = mix(texture(texture1, oTexCoord), texture(texture2, oTexCoord), blendFactor); 

    // Multiply the blended color with the fragment's color
    FragColor = blendedColor * oFragColor;
    
    // You can also uncomment these lines if needed
    // vec3 normal = vNormal;
    // FragColor = mix(texture(texture1, oTexCoord), texture(texture2, oTexCoord), 0.2);
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
