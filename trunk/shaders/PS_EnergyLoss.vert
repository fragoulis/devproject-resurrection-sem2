//*****************************************************************************
//**
//**   File:               PS_EnergyLoss.vert
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-20 12:31:07 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 205 $
//**
//*****************************************************************************

uniform float currentTime;
uniform float particleLife;
uniform float particleSize;
uniform float systemLife;
uniform int particleColor;
attribute vec4 velocity;
attribute vec4 offset;

varying vec4 color;
varying vec2 vTexCoord;

const vec3 scale = vec3(1.0)*particleSize;
const float partLifeDenom = 1.0 / particleLife;
const float sum_time = currentTime + particleLife - systemLife;


void main(void)
{	
	gl_TexCoord[0].st = 0.5*(gl_Vertex.xy + vec2(1.0));
	
	vec4  vert = vec4(0.0);
    const float tmpTime = currentTime - velocity.w;
	float t = mod(tmpTime,particleLife);
	if(sum_time <= velocity.w)
    {
		// Add the start offset & the time-based velocity
        vert    = vec4(velocity.xyz*t, 1.0);
        if (particleColor == 0)
			color = vec4(1.0,0, 0, 1); 
		else if (particleColor == 1)
			color = vec4(1.0,1.0, 0.0, 1); 
		else if (particleColor == 2)
			color = vec4(0.0,0.0, 1.0, 1); 
					 
		vert.xyz += offset.xyz;
		vert += velocity*currentTime;

		vert = gl_ModelViewMatrix*vert;
		vert.xyz += gl_Vertex.xyz*scale;
    }
    else
		color.w = -1.0;
		
	vTexCoord = gl_Vertex.xy;
 
    gl_Position = gl_ProjectionMatrix * vert;
}