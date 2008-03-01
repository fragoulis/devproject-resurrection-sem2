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