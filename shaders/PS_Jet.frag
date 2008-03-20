//*****************************************************************************
//**
//**   File:               PS_Jet.frag
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-14 17:14:30 +0000 (ven, 14 mar 2008) $
//**   Last Revision:      $Revision: 166 $
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