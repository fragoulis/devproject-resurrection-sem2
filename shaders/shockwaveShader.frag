uniform sampler2D sourceTex;

uniform float time;

const float PI = 3.1415926535897932384626433832795;
const float PI2 = 2.0 * PI;
const float DEG2RAD = PI / 180.0;

// width in terms of texcoords
const float WIDTH = 0.01;

void main()
{
	vec2 newTexCoords = (gl_TexCoord[0].st - 0.5)*time + 0.5;
	
	vec4 color = texture2D(sourceTex,newTexCoords);
	gl_FragColor = color;
}