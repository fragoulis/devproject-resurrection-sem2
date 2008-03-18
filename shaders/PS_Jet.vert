//*****************************************************************************
//**
//**   File:               PS_Jet.vert
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-16 19:42:46 +0000 (dom, 16 mar 2008) $
//**   Last Revision:      $Revision: 176 $
//**
//*****************************************************************************

uniform float currentTime;
uniform float particleLife;
uniform float particleSize;
uniform float systemLife;
attribute vec4 velocity;
//attribute vec4 offset;

varying vec4 color;

const vec3 scale = vec3(1.0)*particleSize;
const float partLifeDenom = 1.0 / particleLife;
const float sum_time = currentTime + particleLife - systemLife;

//const vec3 ACCELERATION = vec3(0.0,-9.8,0.0);

void main(void)
{	
	gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	
	vec4  vert = vec4(velocity.xyz,0.0);
    const float tmpTime = currentTime - velocity.w;
	float t = mod(tmpTime,particleLife);
	if(sum_time <= velocity.w)
    {
		// Add the start offset & the time-based velocity
        vert    += vec4(velocity.xyz*t , 1.0);
        vert.x *= 5;
        vert.z +=  velocity.z*t*15;  
        const float to_draw = t*partLifeDenom;	// know how far in it's life has passed (percent)
        const float to_draw2 = to_draw * to_draw;
        const float quad_func = 1.0 - to_draw2;
        color = vec4(1.0,
					 0.1 + 0.2*quad_func,
					 0.1*quad_func,
					 quad_func);

		//vert.xyz += offset.xyz;
		vert = gl_ModelViewMatrix*vert;
		vert.xyz += gl_Vertex.xyz*scale;
    }
    else
		color.w = -1.0;
		
		
 
    gl_Position = gl_ProjectionMatrix * vert;
}