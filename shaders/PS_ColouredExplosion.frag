uniform sampler2D particleTex;

varying float vColor;
varying vec2 vTexCoord;
varying vec4 alphaColor;

void main(void)
{
	float fade = 1.0 - pow(dot(vTexCoord, vTexCoord), 0.37);
	gl_FragColor = fade * texture2D(particleTex, vec2(vColor,0.0));
	gl_FragColor.a = alphaColor.a* fade;

}