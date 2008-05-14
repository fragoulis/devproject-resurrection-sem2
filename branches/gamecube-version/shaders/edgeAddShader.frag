uniform sampler2D sourceTex;
uniform sampler2D edgeTex;

void main()
{
	vec4 source = texture2D(sourceTex,gl_TexCoord[0].st);
	vec4 edge = texture2D(edgeTex,gl_TexCoord[0].st);
	gl_FragColor = source - vec4(edge.r);
}