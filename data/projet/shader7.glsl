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
const float h=3;
const float r=1;

uniform sampler2D diffuse_color;
uniform vec3 source;
uniform vec3 direction;

out vec4 fragment_color;

vec4 CelShading ( vec4 color, vec3 LightDir ,int i)
{
	float Intensity = dot( LightDir , normalize(n) );
	float factor = 0.0;
	if(i==1)
	{
		if ( Intensity < 0.25 ) factor = 0.25;
		if ( Intensity > 0.25 && Intensity <0.5 ) factor = 0.5;
		if ( Intensity > 0.5 && Intensity <0.75 ) factor = 0.75;
		if ( Intensity > 0.75 ) factor = 1;
	}
	color *= vec4 ( factor, factor, factor, 1.0 );

	return color;
} 

void main( )
{
    vec3 camera= vec3(viewInvMatrix * vec4(0, 0, 0, 1));        // position de la camera dans le repere du monde

    //vec3  colore = vec3(0,0,1);
    float cone_dist = dot(p - source,direction);
    float cone_radius = (cone_dist / h) * r;
    float orth_distance = length((p - source) - cone_dist * direction);
   
    int i=0;
    if(orth_distance<cone_radius)
    {
	i=1;
    }
    if(cone_dist>3)
    {
	i=0;
    }

    
    vec4 colore= texture(diffuse_color, vertex_texcoord);

    vec3 lightDir = normalize(-direction);
   
    vec4 lighting = CelShading(colore,lightDir,i);  
    
    fragment_color= lighting;
}
#endif

