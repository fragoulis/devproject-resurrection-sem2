uniform sampler2D texmap0;	
uniform sampler2D texmap1; 
uniform sampler2D shadowTex; 
uniform samplerRect contribMap;	

void main()
{
	float contrib = textureRect(contribMap,gl_TexCoord[1].st).r;
	vec4 mapColor = mix(texture2D(texmap0,gl_TexCoord[0].st),
						texture2D(texmap1,gl_TexCoord[0].st),
						contrib);
	mapColor *= (1.0 - texture2DProj(shadowTex, gl_TexCoord[2]));
	gl_FragColor = mapColor;
}