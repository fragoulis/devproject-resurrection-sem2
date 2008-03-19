uniform vec4 color;
uniform sampler2D flareTex;

void main()
{
	gl_FragColor = texture2D(flareTex,gl_TexCoord[0].st).r * color;
}