uniform sampler2D contributionTex;
uniform sampler2D layer1map;
uniform sampler2D layer2map;
uniform sampler2D layer3map;
uniform sampler2D layer4map;
uniform vec4 coordModifier;
uniform sampler2D lightmap;

uniform vec4 lightAmbDiff;

uniform float step;

const float step2 = 2.0 * step;
const float step3 = 3.0 * step;

//#define KERNEL_SIZE 25
#define KERNEL_SIZE 49

/*
const float kernel[KERNEL_SIZE] = { 1.0/273.0, 4.0/273.0, 4.0/273.0, 4.0/273.0, 1.0/273.0,
									4.0/273.0, 16.0/273.0, 28.0/273.0, 16.0/273.0, 4.0/273.0, 
									7.0/273.0, 26.0/273.0, 41.0/273.0, 26.0/273.0, 7.0/273.0,
									4.0/273.0, 16.0/273.0, 28.0/273.0, 16.0/273.0, 4.0/273.0, 
									1.0/273.0, 4.0/273.0, 4.0/273.0, 4.0/273.0, 1.0/273.0 };
*/


const float kernel[KERNEL_SIZE] = { 0.00000067, 	0.00002292, 	0.00019117, 	0.00038771, 	0.00019117, 	0.00002292, 	0.00000067,
									0.00002292, 	0.00078633, 	0.00655965, 	0.01330373, 	0.00655965, 	0.00078633, 	0.00002292,
									0.00019117, 	0.00655965, 	0.05472157, 	0.11098164, 	0.05472157, 	0.00655965, 	0.00019117,
									0.00038771, 	0.01330373, 	0.11098164, 	0.22508352, 	0.11098164, 	0.01330373, 	0.00038771,
									0.00019117, 	0.00655965, 	0.05472157, 	0.11098164, 	0.05472157, 	0.00655965, 	0.00019117,
									0.00002292, 	0.00078633, 	0.00655965, 	0.01330373, 	0.00655965, 	0.00078633, 	0.00002292,
									0.00000067, 	0.00002292, 	0.00019117, 	0.00038771, 	0.00019117, 	0.00002292, 	0.00000067};

/*
const vec2 offset[KERNEL_SIZE] = { vec2(-step2, -step2), vec2(-step, -step2), vec2(0.0, -step2), vec2(step, -step2), vec2(step2, -step2),
									vec2(-step2, -step), vec2(-step, -step), vec2(0.0, -step), vec2(step, -step), vec2(step2, -step),
									vec2(-step2, 0.0), vec2(-step, 0.0), vec2(0.0, 0.0), vec2(step, 0.0), vec2(step2, 0.0),
									vec2(-step2, step), vec2(-step, step), vec2(0.0, step), vec2(step, step), vec2(step2, step),
									vec2(-step2, step2), vec2(-step, step2), vec2(0.0, step2), vec2(step, step2), vec2(step2, step2) };
*/

const vec2 offset[KERNEL_SIZE] = { vec2(-step3,-step3) , vec2(-step2,-step3) , vec2(-step,-step3) , vec2(0.0,-step3) , vec2(step,-step3) , vec2(step2,-step3) , vec2(step3,-step3) ,
									vec2(-step3,-step2) , vec2(-step2,-step2) , vec2(-step,-step2) , vec2(0.0,-step2) , vec2(step,-step2) , vec2(step2,-step2) , vec2(step3,-step2) ,
									vec2(-step3,-step) , vec2(-step2,-step) , vec2(-step,-step) , vec2(0.0,-step) , vec2(step,-step) , vec2(step2,-step) , vec2(step3,-step) ,
									vec2(-step3, 0.0) , vec2(-step2,0.0) , vec2(-step,0.0) , vec2(0.0,0.0) , vec2(step,0.0) , vec2(step2,-step3) , vec2(step3,0.0) ,
									vec2(-step3,step) , vec2(-step2,step) , vec2(-step,step) , vec2(0.0,step) , vec2(step,step) , vec2(step2,step) , vec2(step3,step) ,
									vec2(-step3,step2) , vec2(-step2,step2) , vec2(-step,step2) , vec2(0.0,step2) , vec2(step,step2) , vec2(step2,step2) , vec2(step3,step2) ,
									vec2(-step3,step3) , vec2(-step2,step3) , vec2(-step,step3) , vec2(0.0,step3) , vec2(step,step3) , vec2(step2,step3) , vec2(step3,step3) };
									

float blur();

void main()
{
	vec4 contrib = texture2D(contributionTex,gl_TexCoord[0].st);
	vec4 layer1color = texture2D(layer1map,gl_TexCoord[0].st*coordModifier.x)*
					   contrib.r;
	vec4 layer2color = texture2D(layer2map,gl_TexCoord[0].st*coordModifier.y)*
					   contrib.g;
	vec4 layer3color = texture2D(layer3map,gl_TexCoord[0].st*coordModifier.z)*
					   contrib.b;
	vec4 layer4color = texture2D(layer4map,gl_TexCoord[0].st*coordModifier.w)*
					   contrib.a;
					 
	float lightmapcontrib = blur();
	gl_FragColor = lightmapcontrib*(layer1color + layer2color + layer3color + layer4color)*lightAmbDiff;
}

float blur()
{
	float sum = 0.0;
	for(int i=0; i<KERNEL_SIZE; i++ )
	{
		sum += texture2D(lightmap, gl_TexCoord[0].st + offset[i]).r * kernel[i];
	}
	return sum;
}