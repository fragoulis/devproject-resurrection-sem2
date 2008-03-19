varying vec2 vcoord;

void main()
{
	vcoord = gl_Vertex.xy;
	gl_Position = ftransform();
}