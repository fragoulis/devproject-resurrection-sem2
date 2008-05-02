uniform sampler2D sourceTex;

uniform float width;
uniform float height;

const float step_w = 1.0/width;
const float step_h = 1.0/height;

#define KERNEL_SIZE 9
const float kernel[KERNEL_SIZE] = { 2.0, 1.0, 2.0,
	                                1.0, -12.0, 1.0,
									2.0, 1.0, 2.0 };

				
const vec2 offset[KERNEL_SIZE] = { vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h), 
				vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0), 
				vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) };

void main()
{
	int i = 0;
    //vec4 sum = texture2D(sourceTex, gl_TexCoord[0].st + offset[1])*kernel[1];
    //sum += texture2D(sourceTex, gl_TexCoord[0].st + offset[3])*kernel[3];
    //sum += texture2D(sourceTex, gl_TexCoord[0].st + offset[4])*kernel[4];
    //sum += texture2D(sourceTex, gl_TexCoord[0].st + offset[5])*kernel[5];
    //sum += texture2D(sourceTex, gl_TexCoord[0].st + offset[7])*kernel[7];
    vec4 sum = vec4(0.0);
    
    for( i=0; i<KERNEL_SIZE; i++ )
	{
		vec4 tmp = texture2D(sourceTex, gl_TexCoord[0].st + offset[i]);
		sum += tmp * kernel[i];
	}

	
	gl_FragColor = sum;

}