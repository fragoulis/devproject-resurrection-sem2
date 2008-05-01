uniform sampler2D sourceTex;

uniform float width;
uniform float height;

const float step_w = 1.0/width;
const float step_h = 1.0/height;

#define KERNEL_SIZE 9
const float kernel[KERNEL_SIZE] = { 0.0, 1.0, 0.0,
	                                1.0, -4.0, 1.0,
									0.0, 1.0, 0.0 };

				
const vec2 offset[KERNEL_SIZE] = { vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h), 
				vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0), 
				vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) };

void main()
{
	int i = 0;
    vec4 sum = vec4(0.0);
	for( i=0; i<KERNEL_SIZE; i++ )
	{
		vec4 tmp = texture2D(sourceTex, gl_TexCoord[0].st + offset[i]);
		sum += tmp * kernel[i];
	}
	
	sum = 0.0001*sum + texture2D(sourceTex, gl_TexCoord[0].st);
	
	gl_FragColor = sum;

}