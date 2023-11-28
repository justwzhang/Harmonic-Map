#version 330

// data from vertex shader
in vec3 o_normal;
in vec3 o_toLight;
in vec3 o_toLight2;
in vec3 o_toCamera;
in vec2 o_texcoords;

// parameters of the light and possible values
uniform vec3 u_lightAmbientIntensitys; // = vec3(0.6, 0.3, 0);
uniform vec3 u_lightDiffuseIntensitys; // = vec3(1, 0.5, 0);
uniform vec3 u_lightSpecularIntensitys; // = vec3(0, 1, 0);

// parameters of the material and possible values
uniform vec3 u_matAmbientReflectances; // = vec3(1, 1, 1);
uniform vec3 u_matDiffuseReflectances; // = vec3(1, 1, 1);
uniform vec3 u_matSpecularReflectances; // = vec3(1, 1, 1);
uniform float u_matShininess; // = 64;

/////////////////////////////////////////////////////////

// texture with diffuese color of the object
uniform sampler2D u_texture;

// color for framebuffer
out vec4 resultingColor;

/////////////////////////////////////////////////////////

// returns intensity of reflected ambient lighting
vec3 ambientLighting()
{
   return u_matAmbientReflectances * u_lightAmbientIntensitys;
}

// returns intensity of diffuse reflection
vec3 diffuseLighting(in vec3 N, in vec3 L)
{
   // calculation as for Lambertian reflection
   float diffuseTerm = clamp(dot(N, L), 0, 1) ;
   return u_matDiffuseReflectances * u_lightDiffuseIntensitys * diffuseTerm;
}

// returns intensity of specular reflection
vec3 specularLighting(in vec3 N, in vec3 L, in vec3 V)
{
   float specularTerm = 0;

   // calculate specular reflection only if
   // the surface is oriented to the light source
   if(dot(N, L) > 0)
   {
      // half vector
      vec3 H = normalize(L + V);
      specularTerm = pow(dot(N, H), u_matShininess);
   }
   return u_matSpecularReflectances * u_lightSpecularIntensitys * specularTerm;
}

void main(void)
{
   // normalize vectors after interpolation
   vec3 L = normalize(o_toLight);
   vec3 V = normalize(o_toCamera);
   vec3 N = normalize(o_normal);

   // get Blinn-Phong reflectance components
   vec3 Iamb = ambientLighting();
   vec3 Idif = diffuseLighting(N, L);
   vec3 Ispe = specularLighting(N, L, V);

   // diffuse color of the object from texture
   //vec3 diffuseColor = texture(u_texture, o_texcoords).rgb;
   vec3 diffuseColor = vec3(229.0 / 255.0f + 0.2f, 162.0 / 255.0f, 141.0 / 255.0f);

   vec3 L2 = normalize(o_toLight2);
   vec3 V2 = normalize(o_toCamera);
   vec3 N2 = normalize(o_normal);

   // get Blinn-Phong reflectance components
   vec3 Iamb2 = ambientLighting();
   vec3 Idif2 = diffuseLighting(N2, L2);
   vec3 Ispe2 = specularLighting(N2, L2, V2);

   // combination of all components and diffuse color of the object
   
   resultingColor = texture(u_texture, o_texcoords);
   resultingColor.xyz = 0.9 * resultingColor.xyz 
                      + 0.2 * diffuseColor * (Iamb + Idif + Ispe)
                      + 0.2 * diffuseColor * (Iamb2 + Idif2 + Ispe2);
   
   resultingColor.a = 0.8;
}
