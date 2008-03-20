//*****************************************************************************
//**
//**   File:               perpixeltex.vert
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

uniform vec4 lightPosition;
uniform vec4 lightColor;
varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,viewVec;

void main()
{	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize((gl_ModelViewMatrix * vec4(lightPosition.xyz,0.0)).xyz);
	
	diffuse = gl_FrontMaterial.diffuse * lightColor;
	ambient = 0.1 * lightColor * gl_FrontMaterial.ambient;
	
	viewVec = (gl_ModelViewMatrix * gl_Vertex).xyz;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
		
	gl_Position = ftransform();	
} 
