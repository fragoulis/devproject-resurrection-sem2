uniform sampler2D tex;
uniform float transparency;

void main()
{
	vec4 texColor = texture2D(tex,gl_TexCoord[0].st);
	
	gl_FragColor = texColor;
	if (texColor.r <= 0.1 && texColor.g <= 0.1 && texColor.b <= 0.1)
		gl_FragColor.a = texColor.r;
	gl_FragColor.a *= transparency;
	
}