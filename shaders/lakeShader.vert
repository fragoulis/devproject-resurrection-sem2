uniform vec2 terrain_tex_scale;
uniform vec2 terrain_tex_offset;
uniform vec4 lightPosition;

uniform float timer;
uniform float waveSpeed;
uniform float waveChangeRate;
uniform float waveRepeats;
uniform vec2 waveDir;

varying vec3 viewVec;
varying vec3 lightVec;
varying vec3 normal;


varying mat3 tbn;

void main()
{
	//projected tex coords calculation
	gl_TexCoord[1].st = gl_MultiTexCoord0.st*terrain_tex_scale - terrain_tex_offset;
	
	gl_TexCoord[0].st = gl_MultiTexCoord0.st*waveRepeats*vec2(-1.0,1.0) + waveDir*waveSpeed*timer;
	gl_TexCoord[0].p = waveChangeRate*timer;	// Noise tex coords
	
	gl_TexCoord[1].pq = gl_MultiTexCoord0.st;					// height tex coords
	
	viewVec = (gl_ModelViewMatrix * gl_Vertex).xyz;
	lightVec = normalize((gl_ModelViewMatrix * (-lightPosition)).xyz);
	
	normal = gl_NormalMatrix * vec3(0.0,1.0,0.0);
	
	tbn = gl_NormalMatrix * mat3(vec3(1.0,0.0,0.0),
								 vec3(0.0,0.0,-1.0),
								 vec3(0.0,1.0,0.0));
	
	gl_Position = ftransform();
}