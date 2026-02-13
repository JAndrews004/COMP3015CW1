#version 460

in vec3 LightIntensity;
in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

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


vec3 phongModel(int light, vec3 position,vec3 n)
{
    vec3 ambient = lights[light].La * Material.Ka;

    vec3 s = normalize(vec3(lights[light].Position.xyz -position));
    float sDotN = max(dot(s,n),0.0);
    //vec3 diffuse = Material.Kd  * floor(sDotN*levels)*scaleFactor; // toon shading
    vec3 diffuse = Material.Kd  *sDotN;

    vec3 spec = vec3(0.0);
    if(sDotN >0){
        vec3 v=normalize(-position.xyz);
        vec3 h = normalize(s + v);
        spec = Material.Ks * pow(max(dot(n, h), 0.0), Material.Shininess);
     }
     return ambient + lights[light].L * diffuse + spec * lights[light].L;

}

void main() {
    
    vec3 lighting = vec3(0.0f);
    vec3 n =normalize(Normal);
    for(int i =0;i<4;i++)
    {
        lighting += phongModel(i,Position,n);
    }
    FragColor = vec4(lighting,1);
}
