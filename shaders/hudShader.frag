uniform sampler2D tex;
uniform float transparency;

void main()
{
	vec4 texColor = texture2D(tex,gl_TexCoord[0].st);
	
	gl_FragColor = texColor;
	gl_FragColor.a = texColor.r;
	gl_FragColor.a *= transparency;
	
}