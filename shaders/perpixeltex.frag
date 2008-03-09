//*****************************************************************************
//**
//**   File:               perpixeltex.frag
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;

uniform sampler2D texmap0;


void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = ambient;
	
	vec4 texcolor = texture2D(texmap0,gl_TexCoord[0].st);
	
	n = normalize(normal);
	
	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += diffuse * NdotL;
		color *= texcolor;
		color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}

	gl_FragColor = color;
}