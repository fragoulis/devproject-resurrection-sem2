//*****************************************************************************
//**
//**   File:               PS_Explosion.vert
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

uniform float currentTime;
uniform float particleLife;
uniform float particleSize;
uniform float systemLife;
attribute vec4 velocity;
attribute vec4 offset;

varying vec4 color;
varying float vColor;
varying vec2 vTexCoord;

const vec3 scale = vec3(1.0)*particleSize;
const float partLifeDenom = 1.0 / particleLife;
const float sum_time = currentTime + particleLife - systemLife;

const vec3 GRAVITY = vec3(0.0,-9.8,0.0);

void main(void)
{	
	gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	
	vec4  vert = vec4(0.0);
    const float tmpTime = currentTime - velocity.w;
	float t = mod(tmpTime,particleLife);
	if(sum_time <= velocity.w)
    {
		// Add the start offset & the time-based velocity
        vert    = vec4(velocity.xyz*t + 0.5*GRAVITY*t*t, 1.0);
        const float to_draw = t*partLifeDenom;	// know how far in it's life has passed (percent)
        const float to_draw2 = to_draw * to_draw;
        const float quad_func = 1.0 - to_draw2;
        color = vec4(1.0,
					 0.1 + 0.2*quad_func,
					 0.1*quad_func,
					 quad_func);

		vert.xyz += offset.xyz;
		vert.xyz += velocity*currentTime;
		vert.xyz *= cos(currentTime);
		vert.z *= sin(currentTime);
		//vert.x += cos(currentTime)*vert.x + sin(currentTime)*vert.z;
		//vert.z += cos(currentTime)*vert.z - sin(currentTime)*vert.x;
		vert = gl_ModelViewMatrix*vert;
		vert.xyz += gl_Vertex.xyz*scale;
    }
    else
		color.w = -1.0;
		
		//vColor = partLifeDenom;
		
		//vTexCoord = gl_Vertex.xy;
		
 
    gl_Position = gl_ProjectionMatrix * vert;
}