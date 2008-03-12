uniform float timePercent;
uniform sampler2D texmap0;

const float OFFSET = 0.1;
const float ONE_MINUS_OFFSET = 1.0 - OFFSET;

void main()
{
	// distFromCenter : 0 .. 0.5, max at edges
	float distFromCenter = abs(gl_TexCoord[0].s - 0.5);				
	// boundary = 0 .. 0.5, max at center height
	float tmp = abs(gl_TexCoord[0].t - 0.5);
	float boundary = ONE_MINUS_OFFSET*min(timePercent,1.0)*(0.25 - tmp*tmp);
	
	vec3 color = texture2D(texmap0,gl_TexCoord[0].st+vec2(timePercent*0.1)).rgb;
	
	gl_FragColor = vec4(color,1.0 - smoothstep(distFromCenter + OFFSET,distFromCenter,boundary));
}