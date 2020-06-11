#version 330


#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normal;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

out vec3 p;
out vec3 n;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    
    p= vec3(modelMatrix * vec4(position, 1));
    n= mat3(modelMatrix) * normal;
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 p;
in vec3 n;

uniform mat4 viewInvMatrix;

//~ const vec3 source= vec3(0, 0, 0);   // source dans le repere du monde
const vec3 emission= vec3(1);
const float k= 0.8;
const float alpha=0.5;
const float F0=4;
const float PI= 3.14159265359;

out vec4 fragment_color;
void main( )
{
    vec3 camera= vec3(viewInvMatrix * vec4(0, 0, 0, 1));        // position de la camera dans le repere du monde
    vec3 source= vec3(viewInvMatrix * vec4(10, 10, 0, 1));        // source "frontale" positionnee par rapport a la camera
    
    // directions
vec3 o= normalize(camera - p);
vec3 l= normalize(source - p);
vec3 h= normalize(o + l);
// cos
vec3 nn= normalize(n);
float cos_theta= max(0, dot(nn, l));
float cos_theta_h= max(0, dot(nn, h));
float cos_theta_o= max(0, dot(nn, o));
// meme hemisphere
if(cos_theta <= 0 || cos_theta_h <= 0 || cos_theta_o <= 0)
    return;
// D
float D= alpha / (1 + cos_theta_h*cos_theta_h * (alpha*alpha -1));
D= D*D / PI;
// G2
float tan2_theta_o= 1 / (cos_theta_o*cos_theta_o) - 1;
float lambda_o= 1 + alpha*alpha * tan2_theta_o;
float tan2_theta= 1 / (cos_theta*cos_theta) - 1;
float lambda= 1 + alpha*alpha * tan2_theta;
float G2= 2 / (sqrt(lambda_o) + sqrt(lambda));
// F
float F= F0 + (1 - F0) * pow(1 - dot(o, h), 5);
// brdf

vec3  colore = vec3(1,0.84,0);
float fr= (F * D * G2) / (4 * cos_theta_o * cos_theta);
vec3 color= colore*emission * fr * cos_theta;    
    
    fragment_color= vec4(color, 1);
}
#endif

