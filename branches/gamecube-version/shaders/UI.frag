//*****************************************************************************
//**
//**   File:               UI.frag
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

uniform sampler2D colorMap;
uniform sampler2D shadowMap;

void main(void)
{
	vec4 color = vec4(1,1,1,1);
	color *= texture2D(colorMap, gl_TexCoord[0].st);
	vec4 shadowColor = texture2DProj(shadowMap, gl_TexCoord[1]);

	if (shadowColor.r < 0.1)
		color *= shadowColor+0.5;
		
	gl_FragColor = color;
}