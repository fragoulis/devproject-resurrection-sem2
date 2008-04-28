uniform sampler2D particleTex;
uniform sampler3D particleNoise;
varying vec4 color;

void main(void)
{
	float lum = texture2D(particleTex,gl_TexCoord[0].st).r;
	float intense = texture3D(particleNoise,gl_TexCoord[1].stp).r;

	vec3 colour = intense*color.xyz*color.xyz*4.0;
	
	float alpha = lum*color.a*intense*intense;
	
	gl_FragColor = vec4(colour,alpha);
}