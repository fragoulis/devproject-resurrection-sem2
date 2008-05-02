uniform sampler2D tex;
uniform float percentage;

void main()
{
	gl_FragColor = texture2D(tex,gl_TexCoord[0].st)*percentage;
}