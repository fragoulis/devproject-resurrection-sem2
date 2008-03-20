//*****************************************************************************
//**
//**   File:               PS_Explosion.frag
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-20 12:45:37 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 209 $
//**
//*****************************************************************************

uniform sampler2D particleTex;
varying vec4 color;
varying vec2 vTexCoord;


void main(void)
{
	float lum = texture2D(particleTex,gl_TexCoord[0].st).r;
	gl_FragColor = lum*color*vec4(vec3(1.0),lum);
}