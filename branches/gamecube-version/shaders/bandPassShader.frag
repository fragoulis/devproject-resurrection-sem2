uniform sampler2D sourceTex;

const float COLOR_SHADES = 48.0;
const float COLOR_SHADES_INCREMENT = 1.5 / COLOR_SHADES; 

void main()
{
	vec3 color = texture2D(sourceTex,gl_TexCoord[0].st).rgb;
	
	vec3 outcolor = vec3(ivec3(color*COLOR_SHADES))*COLOR_SHADES_INCREMENT;
	
	gl_FragColor = vec4(outcolor,1.0);
}