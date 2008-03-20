//*****************************************************************************
//**
//**   File:               PS_Explosion.frag
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

uniform sampler2D particleTex;
varying vec4 color;
varying vec2 vTexCoord;


void main(void)
{
	float fade = pow(dot(vTexCoord, vTexCoord), 0.5);
	gl_FragColor = (1.0-fade)*texture2D(particleTex,gl_TexCoord[0].st)*color;
}