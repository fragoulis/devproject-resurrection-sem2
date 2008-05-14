uniform sampler2D texmap0;
varying vec4 color;

void main()
{
	gl_FragColor = color * texture2D(texmap0,gl_TexCoord[0].st);
}