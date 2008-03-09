//*****************************************************************************
//**
//**   File:               UI.frag
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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