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
uniform float shipSpeed;
attribute vec4 velocity;
//attribute vec4 offset;

varying vec4 color;
varying vec2 vTexCoord;

const vec3 scale = vec3(1.0)*particleSize;
const float partLifeDenom = 1.0 / particleLife;
const float sum_time = currentTime + particleLife - systemLife;

vec3 acceleration = vec3(0.0,0,50.0);

void main(void)
{	
	gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	
	vec4  vert = vec4(velocity.xyz,0.0);
    const float tmpTime = currentTime - velocity.w;
	float t = mod(tmpTime,particleLife);
	if(sum_time <= velocity.w)
    {
		//acceleration.z = (1.0-velocity.y)*50.0;
		acceleration.z += (1.0-abs(velocity.x))*100.0;
		acceleration.z += (shipSpeed + 1.0)/2.0;
        vert    += vec4(velocity.xyz*t + acceleration*t*t*0.5 , 1.0);
        vert.x *= 4.0;
        vert.z +=  velocity.z*t; 
        vert.z = abs(vert.z); 
        vert.z += 30.0; //OFFSET
        const float to_draw = t*partLifeDenom;	// know how far in it's life has passed (percent)
        const float to_draw2 = to_draw * to_draw;
        const float quad_func = 1.0 - to_draw2;
        color = vec4(1.0,
					 0.1 + 0.2*quad_func,
					 0.1*quad_func,
					 quad_func);
					 
		/*color = vec4(0.5,
			0.1 + 0.2*(1.0-to_draw),
			0.7,
			quad_func);*/

		vert = gl_ModelViewMatrix*vert;
		vert.xyz += gl_Vertex.xyz*scale;
    }
    else
		color.w = -1.0;
		
	vTexCoord = gl_Vertex.xy;	
 
    gl_Position = gl_ProjectionMatrix * vert;
}