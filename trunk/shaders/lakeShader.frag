uniform sampler2D lakeTex;
uniform sampler2D shadowTex; 	

void main()
{
	vec4 mapColor = texture2D(lakeTex,gl_TexCoord[0].st)*(1.0 - texture2DProj(shadowTex, gl_TexCoord[1]));
	gl_FragColor = mapColor;
}