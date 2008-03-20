void main()
{
	gl_TexCoord[0].st = gl_MultiTexCoord0;
	gl_Position = ftransform();
}