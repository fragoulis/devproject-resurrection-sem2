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


void main(void)
{
	gl_FragColor = texture2D(particleTex,gl_TexCoord[0].st)*color;
}