#version 460

in vec3 LightIntensity;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 NormalInterp;


layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D Tex1;
layout (binding = 1) uniform sampler2D NormalMap;

uniform struct LightInfo{
    vec4 Position;
    vec3 La;
    vec3 L;
}lights[4];

uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
}Material;


const int levels = 4;
const float scaleFactor = 1.0/levels;


vec3 phongModel(int light, vec3 position,vec3 n,vec3 texColour)
{
    float distance = length(lights[light].Position.xyz - Position);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.08 * distance*distance);
    //float attenuation = 1.0;
    vec3 ambient = lights[light].La * Material.Ka * texColour;

    vec3 s = normalize(vec3(lights[light].Position.xyz -position));
    float sDotN = max(dot(s,n),0.0);
    //vec3 diffuse = Material.Kd  * floor(sDotN*levels)*scaleFactor; // toon shading
    vec3 diffuse = Material.Kd  *sDotN * texColour * attenuation;

    vec3 spec = vec3(0.0);
    if(sDotN >0){
        vec3 v=normalize(-position.xyz);
        vec3 h = normalize(s + v);
        spec = Material.Ks * pow(max(dot(n, h), 0.0), Material.Shininess) * attenuation;
     }
     return ambient + lights[light].L * diffuse + spec * lights[light].L;
     
}

void main() {
     mat3 TBN = mat3(
    normalize(Tangent),
    normalize(Bitangent),
    normalize(NormalInterp)
    );

    float TextureSF = 8.0;
    float NormalMapSF = 2.0;
   

    vec3 lighting = vec3(0.0f);
    vec3 nMap = texture(NormalMap, TexCoord*NormalMapSF).rgb;

    nMap = normalize(nMap * 2.0 - 1.0); //Convert from [0,1] to [-1,1]
    vec3 n = normalize(TBN * nMap);
    //vec3 n = normalize(Normal);

    vec3 texColour = texture(Tex1,TexCoord*TextureSF).rgb;
    for(int i =0;i<4;i++)
    {
        lighting += phongModel(i,Position,n,texColour);
    }

    FragColor = vec4(lighting,1);
    //FragColor = vec4(length(Tangent));

}