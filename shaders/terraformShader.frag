varying vec2 vcoord;

uniform vec2 center;
uniform float sqradius;
uniform float timePercent; 

void main()
{
	float vis = 0.0;
	vec2 vector = (vcoord - center);
	float dist = dot(vector,vector);
	
	if(dist > sqradius)
		discard;
	else
	{
		const float bound = sqradius*timePercent;
		vis = smoothstep(bound,0.5*bound,dist);	
		gl_FragColor = vec4(vis);
	}
}