uniform sampler2D particleTex;

varying float vColor;
varying vec2 vTexCoord;
varying vec4 alphaColor;

void main(void)
{
	float fade = pow(dot(vTexCoord, vTexCoord), 0.37);
	gl_FragColor = (1.0 - fade) * texture2D(particleTex, vec2(vColor,0.0));
	gl_FragColor.a = alphaColor.a;

}