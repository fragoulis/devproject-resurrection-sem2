uniform sampler2D tex;
uniform float transparency;
uniform vec4 constantColor; //used to change colors without using different textures

void main()
{
	vec4 texColor = texture2D(tex,gl_TexCoord[0].st);
	
	gl_FragColor = texColor;
	gl_FragColor *= constantColor;
	if (texColor.r <= 0.01 && texColor.g <= 0.01 && texColor.b <= 0.01)
		gl_FragColor.a = texColor.r;
	gl_FragColor.a *= transparency;
	
}