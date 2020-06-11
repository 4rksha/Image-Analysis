#version 330


#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normal;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec3 p;
out vec3 n;
out vec4 frag;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    
    p= vec3(modelMatrix * vec4(position, 1));
    n= mat3(modelMatrix) * normal;
    frag=gl_Position;
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 p;
in vec3 n;
in vec4 frag;

uniform mat4 viewInvMatrix;

//~ const vec3 source= vec3(0, 0, 0);   // source dans le repere du monde
const vec3 emission= vec3(1);
const float k= 3;
const float alpha=3;
const float PI= 3.14159265359;

out vec4 fragment_color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = 1;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

void main( )
{
    vec3 camera= vec3(viewInvMatrix * vec4(0, 0, 0, 1));        // position de la camera dans le repere du monde
    vec3 source= vec3(viewInvMatrix * vec4(10, 10, 0, 1));        // source "frontale" positionnee par rapport a la camera
    
    // directions 
    vec3 o= normalize(camera - p);
    vec3 l= normalize(source - p);
    vec3 h= normalize(o + l);
    float cos_theta_h= dot(normalize(n), h);
    // cos
    float cos_theta= max(0, dot(normalize(n), l));
    // brdf
    float fr= (alpha+8) / (8*PI) * pow(cos_theta_h, alpha);
    
    // brdf
    float fr2= k / PI;
    vec3  colore = vec3(1,0.84,0);
    vec3 color2= colore*emission * fr2 * cos_theta;
    vec3 color= colore*emission * fr * cos_theta;

    vec3 colorfin=(color2+color)/2;

    vec3 normal = normalize(n);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * colorfin;
    // diffuse
    vec3 lightDir = normalize(source - p);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(camera - p);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(frag);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * colorfin;
    
    fragment_color= vec4(lighting, 1);
}
#endif

