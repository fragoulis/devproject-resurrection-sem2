uniform sampler2D particleTex;
uniform sampler2D shapeTex;

varying float vColor;
varying vec2 vTexCoord;
varying vec4 alphaColor;

void main(void)
{
	float fade = pow(dot(vTexCoord, vTexCoord), 0.5);
	gl_FragColor = texture2D(shapeTex, gl_TexCoord[0].st).r * texture2D(particleTex, vec2(vColor,0.0));
	gl_FragColor.a = alphaColor.a;
}