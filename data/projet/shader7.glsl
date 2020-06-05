#version 330


#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 1) in vec2 texcoord;
layout(location= 2) in vec3 normal;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec3 p;
out vec3 n;
out vec4 frag;
out vec2 vertex_texcoord;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    
    p= vec3(modelMatrix * vec4(position, 1));
    n= mat3(modelMatrix) * normal;
    frag=gl_Position;
    vertex_texcoord=texcoord;
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 p;
in vec3 n;
in vec4 frag;
in vec2 vertex_texcoord;

uniform mat4 viewInvMatrix;

//~ const vec3 source= vec3(0, 0, 0);   // source dans le repere du monde
const vec3 emission= vec3(1);
const float k= 3;
const float alpha=3;
const float PI= 3.14159265359;

uniform sampler2D diffuse_color;

out vec4 fragment_color;

vec4 CelShading ( vec4 color, vec3 LightDir )
{
float Intensity = dot( LightDir , normalize(n) );
float factor = 1.0;
if ( Intensity < 0.25 ) factor = 0.25;
if ( Intensity > 0.25 && Intensity <0.5 ) factor = 0.5;
if ( Intensity > 0.5 && Intensity <0.75 ) factor = 0.75;
if ( Intensity > 0.75 ) factor = 1;
color *= vec4 ( factor, factor, factor, 1.0 );

return color;
} 

void main( )
{
    vec3 camera= vec3(viewInvMatrix * vec4(0, 0, 0, 1));        // position de la camera dans le repere du monde
    vec3 source= vec3(viewInvMatrix * vec4(10, 10, 0, 1));        // source "frontale" positionnee par rapport a la camera

    //vec3  colore = vec3(0,0,1);
    vec4 colore= texture(diffuse_color, vertex_texcoord);

    vec3 lightDir = normalize(source - p);
   
    vec4 lighting = CelShading(colore,lightDir);
    
    fragment_color= lighting;
}
#endif

