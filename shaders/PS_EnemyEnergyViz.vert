uniform float currentTime;
uniform float particleSize;

uniform float tailSize;			// in rads
uniform float shipRadius;
uniform float radsPerSec;
uniform vec4 energyColor;

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
	vec3 posoffset = (cos(radoffset)*xAxis + sin(radoffset)*zAxis)*shipRadius;
	
	// set the final position & transform	
	vec4 new_vertex = gl_ModelViewMatrix * vec4(posoffset,1.0);
	new_vertex.xy += particleSize*gl_Vertex.xy;
	gl_Position = gl_ProjectionMatrix * new_vertex;
	
	color = energyColor;
	color.a = genattrib.x;
}