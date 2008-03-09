//*****************************************************************************
//**
//**   File:               computeCrater.frag
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

uniform samplerRect heightmap;
uniform float linbell_mix;
uniform float bell_area;
uniform float radius;
uniform float oldmap_mix;
uniform vec2 center;
uniform float xzscale;

const float maxHeightDenom = 1.0 / 0.39894228;			// mult by this to get 1 at the max height

void main()
{
	float currentHeight = textureRect(heightmap,gl_TexCoord[0].st).r;
	float distance = length(center - gl_TexCoord[0].st*vec2(xzscale,-xzscale));
	
	if(distance > (radius + bell_area*radius))
	{
		gl_FragColor = vec4(currentHeight);
	}
	else
	{
		float linear_contribution = (1.0 - linbell_mix)*step(distance,radius)*distance/radius;
		
		float distFromEdge = abs(radius - distance);
		
		// find the distance from normal peak in the norm distro domain
		float distCurveDomain = 4.0*distFromEdge / (radius*bell_area);
		
		// fetch the y value from the gaussian func with mu = 0, sigma = 1
		float heightCurveDomain = (1.0 / maxHeightDenom) * exp(-0.5 * distCurveDomain * distCurveDomain);
		
		heightCurveDomain *= maxHeightDenom;
		if(distance > radius)
		{
			float alt_radius = bell_area * radius;
			float oldmix2 = (distance - alt_radius)/alt_radius;
			heightCurveDomain = (1.0 - oldmix2)*heightCurveDomain + oldmix2*currentHeight;
		}
		
		float bell_contribution = linbell_mix*heightCurveDomain;
		
		// mix with old
		float total_height = oldmap_mix*currentHeight + (1.0 - oldmap_mix)*(linear_contribution + bell_contribution);
		
		gl_FragColor = vec4(total_height);
	}
}