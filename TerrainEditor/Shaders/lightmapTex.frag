uniform sampler2D contributionTex;
uniform sampler2D layer1map;
uniform sampler2D layer2map;
uniform sampler2D layer3map;
uniform sampler2D layer4map;
uniform vec4 coordModifier;
uniform sampler2D lightmap;

uniform vec4 lightAmbDiff;

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
					 
	gl_FragColor = texture2D(lightmap,gl_TexCoord[0].st).r*(layer1color + layer2color + layer3color + layer4color)*lightAmbDiff;
}