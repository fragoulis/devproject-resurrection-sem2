uniform float currentTime;
uniform float particleLife;
uniform float particleSize;
uniform float systemLife;
attribute vec4 velocity;

varying float vColor;
varying vec2 vTexCoord;
varying vec4 alphaColor;

const vec3 scale = vec3(1.0)*particleSize;
const float partLifeDenom = 1.0 / particleLife;
const float sum_time = currentTime + particleLife - systemLife;
const float explosionPower = 200.0;


void main(void)
{	
	gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	
	vec4  vert = vec4(0.0);
    const float tmpTime = currentTime - velocity.w;
	float t = mod(tmpTime,particleLife);

	if(sum_time <= velocity.w) {
		// Add the start offset & the time-based velocity
		vert    = vec4(velocity.xyz*t , 1.0);

		vert.xyz += velocity*explosionPower*currentTime;
		vert = gl_ModelViewMatrix*vert;
		vert.xyz += gl_Vertex.xyz*scale;

		vColor = (currentTime/particleLife);
		alphaColor = vec4(1.0);
		alphaColor.a = 1.0-currentTime/systemLife;
		
	} 
		
	
	vTexCoord = gl_Vertex.xy;
		
 
    gl_Position = gl_ProjectionMatrix * vert;
}