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

varying float vColor;
varying vec2 vTexCoord;

void main(void)
{
	//float fade = pow(dot(vTexCoord, vTexCoord), 0.37);
	//gl_FragColor = (1.0 - fade) * texture2D(particleTex, vec2(vColor,0.0));

	//vec4 texColor = texture2D(particleTex,gl_TexCoord[0].st);
	//vec4 texColor = texture2D(particleTex,vec2(vColor,0));
	//gl_FragColor = texColor; //*color;
	gl_FragColor = texture2D(particleTex,gl_TexCoord[0].st)*color;
}