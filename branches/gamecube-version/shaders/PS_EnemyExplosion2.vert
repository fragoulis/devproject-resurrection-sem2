uniform float currentTime;
uniform float particleLife;
uniform float particleSize;
uniform float systemLife;
attribute vec4 velocity;

uniform float sizeModifier;

// TODO : add a modifier for the ship size(e.g radius). The modifier should affect scale, velocity & acceleration

uniform vec4 lightPosition;

varying vec4 color;

const vec3 scale = vec3(1.0)*particleSize;
const float partLifeDenom = 1.0 / particleLife;
const float sum_time = currentTime + particleLife - systemLife;

const vec3 ACCELERATION = vec3(0.0,-900.81*sizeModifier,0.0);

void main(void)
{		
	vec4  vert = vec4(0.0);
    const float tmpTime = currentTime - velocity.w;
	float t = mod(tmpTime,particleLife);
	if(((tmpTime + particleLife) < systemLife) && (tmpTime > 0.0))
    {
		const float to_draw = t*partLifeDenom;	// know how far in it's life has passed (percent)
        const float to_draw2 = to_draw * to_draw;
        const float quad_func = 1.0 - to_draw2;
        
		// Add the start offset & the time-based velocity
		float velt = particleLife*(1 - 1/(1.0 + to_draw*2));
        vert    = vec4(300.0*velocity.xyz*velt*sizeModifier + 0.5*ACCELERATION*t*t, 1.0);     
        
        color = vec4(1.0,
					 //0.1 + 0.2*quad_func,
					 0.2 + 0.5*quad_func,
					 0.1*quad_func,
					 smoothstep(0.0,0.3*particleLife,t)*quad_func);
		
		vert = gl_ModelViewMatrix*vert;
		vert.xyz += gl_Vertex.xyz*scale*sizeModifier*(1.0 - 0.5*pow(2.0,velocity.w - particleLife));
    }
    else
		color.w = -1.0;
 
    gl_Position = gl_ProjectionMatrix * vert;
    
    gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	gl_TexCoord[1].st = gl_TexCoord[0].st*0.8;
	gl_TexCoord[1].p =  t / particleLife;
	gl_TexCoord[1].st += 100*velocity.w/particleLife;
}