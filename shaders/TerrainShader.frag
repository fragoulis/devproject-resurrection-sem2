uniform sampler2D texmap0;	
uniform sampler2D texmap1; 
uniform samplerRect contribMap;	

void main()
{
	vec4 mapColor = texture2D(texmap0,gl_TexCoord[0].st) + texture2D(texmap1,gl_TexCoord[0].st)*textureRect(contribMap,gl_TexCoord[1].st).r;
	gl_FragColor = mapColor;
}