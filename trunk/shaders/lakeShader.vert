uniform vec2 reflectionTexCoordOffsets;
uniform vec2 reflectionTexCoordScale;
uniform vec2 terrain_tex_scale;
uniform vec2 terrain_tex_offset;

uniform float timer;
uniform float waveSpeed;
uniform float waveChangeRate;
uniform float waveRepeats;

varying vec3 viewVec;
varying vec3 lightVec;
varying vec3 normal;
varying vec2 texExtentDenom;

uniform vec4 lightPosition;

void main()
{
	//projected tex coords calculation
	gl_TexCoord[1].st = gl_MultiTexCoord0.st*terrain_tex_scale - terrain_tex_offset;
	
	gl_TexCoord[0].st = gl_MultiTexCoord0.st*waveRepeats*vec2(0.5,1.0) + vec2(waveSpeed*timer);
	gl_TexCoord[0].p = waveChangeRate*timer;	// Noise tex coords
	
	gl_TexCoord[3].st = gl_MultiTexCoord0.st;					// height tex coords
	
	// Reflection tex coords
	gl_TexCoord[2].st = gl_MultiTexCoord0.st*reflectionTexCoordScale - reflectionTexCoordOffsets;	// refl texcoord
	vec4 minMaxReflTexcoords;
	minMaxReflTexcoords.xy = - reflectionTexCoordOffsets;
	minMaxReflTexcoords.zw = reflectionTexCoordScale - reflectionTexCoordOffsets;
	
	texExtentDenom = abs(1.0 / ((minMaxReflTexcoords.zw - minMaxReflTexcoords.xy)*0.5));
	
	viewVec = (gl_ModelViewMatrix * gl_Vertex).xyz;
	lightVec = normalize((gl_ModelViewMatrix * (-lightPosition)).xyz);
	
	normal = gl_NormalMatrix * vec3(0.0,1.0,0.0);
	
	gl_Position = ftransform();
}