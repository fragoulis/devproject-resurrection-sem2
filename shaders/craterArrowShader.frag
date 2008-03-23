uniform sampler2D tex;
uniform vec4 color;

const vec4 white = vec4(0.5,0.5, 0.5 ,0.0);

void main()
{
		
	gl_FragColor = texture2D(tex,gl_TexCoord[0].st)*(color + white);
}