uniform mat4 TexGenMat;
uniform mat4 InvViewMat;

uniform float timer;
uniform float waveSpeed;
uniform float waveChangeRate;
uniform float cloudRepeats;
uniform float waveRepeats;

varying vec3 viewVec;
varying vec3 lightVec;
varying vec3 normal;

uniform vec4 lightPosition;

void main()
{
	//projected tex coords calculation
	vec4 posEye =  gl_ModelViewMatrix * gl_Vertex;
	vec4 posWorld = InvViewMat * posEye;
	gl_TexCoord[1] = TexGenMat * posWorld;
	
	gl_TexCoord[0].st = gl_MultiTexCoord0.st*waveRepeats*vec2(0.5,1.0) + vec2(waveSpeed*timer);
	gl_TexCoord[0].p = waveChangeRate*timer;
	
	gl_TexCoord[2].st = gl_MultiTexCoord0.st * cloudRepeats;
	gl_TexCoord[3].st = gl_MultiTexCoord0.st;
	
	viewVec = (gl_ModelViewMatrix * gl_Vertex).xyz;
	lightVec = normalize((gl_ModelViewMatrix * (-lightPosition)).xyz);
	
	normal = gl_NormalMatrix * vec3(0.0,1.0,0.0);
	
	gl_Position = ftransform();
}