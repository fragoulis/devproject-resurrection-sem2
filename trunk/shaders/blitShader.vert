void main()
{
	gl_TexCoord[0].st = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}