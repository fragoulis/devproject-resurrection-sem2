uniform float mapCellNum;
//uniform mat4 TexGenMat;
//uniform mat4 InvViewMat;

uniform vec2 terrain_tex_scale;
uniform vec2 terrain_tex_offset;

void main()
{
	//projected tex coords calculation

	gl_TexCoord[2].st = gl_MultiTexCoord0.st*terrain_tex_scale - terrain_tex_offset;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1].st = gl_MultiTexCoord0.st * mapCellNum;
	gl_Position = ftransform();
}