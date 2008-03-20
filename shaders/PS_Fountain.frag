//*****************************************************************************
//**
//**   File:               PS_Explosion.frag
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-14 17:14:30 +0000 (ven, 14 mar 2008) $
//**   Last Revision:      $Revision: 166 $
//**
//*****************************************************************************

uniform sampler2D particleTex;
varying vec4 color;
varying vec4 alphaColor;
varying vec2 vTexCoord;

void main(void)
{
	float lum = texture2D(particleTex,gl_TexCoord[0].st).r;
	gl_FragColor = lum*color;
	gl_FragColor.a = alphaColor.a * lum;
}