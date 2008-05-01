uniform samplerRect heightmap;
uniform samplerRect normalmap;
uniform float shadoweffect;
uniform vec3 xyzscale;
uniform vec3 lightdir;
uniform float texsize;
uniform vec2 coord_increment;
uniform float y_increment;

void main()
{
	float height = xyzscale.y*textureRect(heightmap,gl_TexCoord[0].st).r;
	vec3 normal = textureRect(normalmap,gl_TexCoord[0].st).rgb;
	vec2 coords = gl_TexCoord[0].st + coord_increment;
	float shadow_contrib = 0.0;
	do
	{
		float final_terrain_height = xyzscale.y*textureRect(heightmap,coords).r;				
		
		height += y_increment;
		if(final_terrain_height > height)
		{
			shadow_contrib = shadoweffect;
			break;
		}
		coords += coord_increment;
	}while(all(greaterThanEqual(coords,vec2(0,0))) && all(lessThanEqual(coords,vec2(texsize,texsize))));
	
	float NdotL = max(0.0,dot(normal,lightdir));
	float contribution = NdotL*(1.0 - shadow_contrib);

	gl_FragColor = vec4(contribution);
}
