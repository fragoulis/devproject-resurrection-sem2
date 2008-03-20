//*****************************************************************************
//**
//**   File:               UI.vert
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

uniform mat4 TexGenMat;
uniform mat4 InvViewMat;

void main()
{
	//projected tex coords calculation
	vec4 posEye =  gl_ModelViewMatrix * gl_Vertex;
	vec4 posWorld = InvViewMat * posEye;
	gl_TexCoord[1] = TexGenMat * posWorld;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}