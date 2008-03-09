//*****************************************************************************
//**
//**   File:               TexturedLayers.frag
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

uniform samplerRect heightmap;
uniform sampler2D layer1map,layer2map,layer3map,layer4map;
uniform vec3 layerBounds;
uniform vec3 bleedBounds;
uniform float seaLevel;

uniform vec4 texrepeats;

const vec4 sea = vec4(0.0,0.0,0.8,1.0);

void main()
{
	float height = textureRect(heightmap,gl_TexCoord[0].st).r;
	
	vec4 color;
	if(height < seaLevel)
		color = sea;
	else 
	{
		vec4 layer1 = texture2D(layer1map,gl_TexCoord[0].st * texrepeats.x);
		vec4 layer2 = texture2D(layer2map,gl_TexCoord[0].st * texrepeats.y);
		vec4 layer3 = texture2D(layer3map,gl_TexCoord[0].st * texrepeats.z);
		vec4 layer4 = texture2D(layer4map,gl_TexCoord[0].st * texrepeats.w);
		
		float blendArea[6] = {layerBounds.x - bleedBounds.x,
							  layerBounds.x + bleedBounds.x,
							  layerBounds.y - bleedBounds.y,
							  layerBounds.y + bleedBounds.y,
							  layerBounds.z - bleedBounds.z,
							  layerBounds.z + bleedBounds.z};
		if(height < blendArea[0])
			color = layer1;
		else if(height < blendArea[1])
		{
			float pcent = (height - blendArea[0]) / (blendArea[1] - blendArea[0]);
			color = layer2*pcent + layer1*(1.0 - pcent);
		}
		else if(height < blendArea[2])
		{
			color = layer2;
		}
		else if(height < blendArea[3])
		{
			float pcent = (height - blendArea[2]) / (blendArea[3] - blendArea[2]);
			color = layer3*pcent + layer2*(1.0 - pcent);
		}
		else if(height < blendArea[4])
		{
			color = layer3;
		}
		else if(height < blendArea[5])
		{
			float pcent = (height - blendArea[4]) / (blendArea[5] - blendArea[4]);
			color = layer4*pcent + layer3*(1.0 - pcent);
		}
		else
		{
			color = layer4;
		}
		
	}
	gl_FragColor = color;	
}