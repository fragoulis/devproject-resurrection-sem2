//*****************************************************************************
//**
//**   File:               computeContrib.frag
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

uniform samplerRect heightmap;
uniform vec3 layerBounds;
uniform vec3 bleedBounds;

const vec4 layer1 = vec4(0.54,0.3,0.05,1.0);
const vec4 layer2 = vec4(0.0,0.5,0.0,1.0);
const vec4 layer3 = vec4(0.3,0.3,0.3,1.0);
const vec4 layer4 = vec4(1.0,1.0,1.0,1.0);

void main()
{
	float height = textureRect(heightmap,gl_TexCoord[0].st).r;
	
	vec4 color = vec4(0.0);

		
	float blendArea[6] = {layerBounds.x - bleedBounds.x,
						  layerBounds.x + bleedBounds.x,
						  layerBounds.y - bleedBounds.y,
						  layerBounds.y + bleedBounds.y,
						  layerBounds.z - bleedBounds.z,
						  layerBounds.z + bleedBounds.z};
	if(height < blendArea[0])
		color.r = 1.0;
	else if(height < blendArea[1])
	{
		float pcent = (height - blendArea[0]) / (blendArea[1] - blendArea[0]);
		color.rg = vec2(1.0 - pcent,pcent);
	}
	else if(height < blendArea[2])
	{
		color.g = 1.0;
	}
	else if(height < blendArea[3])
	{
		float pcent = (height - blendArea[2]) / (blendArea[3] - blendArea[2]);
		color.gb = vec2(1.0 - pcent,pcent);
	}
	else if(height < blendArea[4])
	{
		color.b = 1.0;
	}
	else if(height < blendArea[5])
	{
		float pcent = (height - blendArea[4]) / (blendArea[5] - blendArea[4]);
		color.ba = vec2(1.0 - pcent,pcent);
	}
	else
	{
		color.a = 1.0;
	}
		

	gl_FragColor = color;	
}