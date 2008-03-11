uniform float mapCellNum;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1].st = gl_MultiTexCoord0.st * mapCellNum;
	gl_Position = ftransform();
}