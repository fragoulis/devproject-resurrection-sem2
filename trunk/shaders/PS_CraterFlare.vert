uniform float currentTime;
uniform float particleSize;

uniform float tailSize;			// in rads
uniform float craterRadius;
uniform float radsPerSec;
uniform float height;
uniform vec4 craterColor;

float pi2 = 6.283185307179586476925286766559;

attribute vec4 genattrib;
/*
	genattrib : 
	
	.x = particle id percentage [0..1]

*/

vec3 xAxis = vec3(1.0,0.0,0.0);
vec3 zAxis = vec3(0.0,0.0,-1.0);

varying vec4 color;

void main()
{
	gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	float radoffset = (genattrib.x*tailSize + radsPerSec*currentTime)*pi2;
	vec3 posoffset = (cos(radoffset)*xAxis + sin(radoffset)*zAxis)*craterRadius;
	posoffset.y = height;
	
	// set the final position & transform	
	vec4 new_vertex = gl_ModelViewMatrix * vec4(posoffset,1.0);
	new_vertex.xy += particleSize*gl_Vertex.xy*genattrib.x*genattrib.x;
	gl_Position = gl_ProjectionMatrix * new_vertex;
	
	color = craterColor;
	color.a = genattrib.x;
}