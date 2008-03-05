uniform sampler2D particleTex;
varying vec4 color;

void main(void)
{
	gl_FragColor = texture2D(particleTex,gl_TexCoord[0].st)*color*vec4(0.4);
}