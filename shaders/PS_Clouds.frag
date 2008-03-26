uniform sampler2D particleTex;
uniform sampler3D noiseTex;

uniform vec4 lightColor;
varying vec3 lightVec;
varying vec3 viewVec;

varying vec3 normal;		// the hacked normal

void main()
{
	float opacity = texture2D(particleTex,gl_TexCoord[0].st).r*0.7;
	float lum = texture3D(noiseTex,gl_TexCoord[1].stp).r*0.5 + 0.5;
	
	vec3 n = normalize(normal);	
	
	float NdotL = max(dot(n,lightVec),0.0);
	vec4 color = (NdotL*NdotL + 0.3)* lightColor*lum;
	//vec4 color = (NdotL*NdotL + 0.3)* lightColor*lum + pow(NdotL,128)*lum;
	color.a = lum*opacity;
	
	gl_FragColor = color;
}