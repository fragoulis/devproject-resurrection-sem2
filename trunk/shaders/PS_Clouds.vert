// Cloud test particle system.

uniform float map_width;	// multiply genattrib.xz by this
uniform float plane_height;	// set gl_Vertex.y as this
uniform float currentTime;		// our currentTime
uniform vec4 wind;		// wind (xz) & magnitude (w)
uniform vec3 wind_tan;		// wind (xz) & magnitude (w)
uniform float particleSize;
uniform float height_scale;

uniform vec4 lightPosition;

attribute vec4 genattrib1;	// starting offsets of the form (-1..1,0,-1..1)
				// in the 4th component we can store the affection by wind
				// in the interval 0..1? 
				// in the 2nd component we can store the deviation to the 
				// wind direction in the interval -1..1
				
attribute vec4 genattrib2;		// stores the height offset in x component
				
				
varying vec3 normal;		// the hacked normal
varying vec3 lightVec;
varying vec3 viewVec;


// mixes deviation vector, wind vector, dt * strength


// We also want them to wrap to the edges of the map

const float PI2 = 6.283185307179586476925286766559;

void main()
{
	// initially set the position
	vec3 position = vec3(genattrib1.x,
						 plane_height + height_scale*genattrib2.x,
						 genattrib1.z);
	position.xz *= map_width;

	// adjust the position by the wind direction & the wind resistance
	
	//position.xz += wind.xz*wind.w*genattrib1.w*currentTime;
	position.xz += currentTime*mix(wind.xz,
									wind_tan.xz*genattrib1.y,
									genattrib1.w)*wind.w;

	// wrap the position in x & z axes
		
	position.x = mod(position.x + map_width,2.0*map_width) -map_width;
	position.z = mod(position.z + map_width,2.0*map_width) -map_width; 


	// set the tex coords
	gl_TexCoord[0].st = gl_Vertex.xy*0.5 + 0.5;

	// set the noise coord, good for a uniform parameter for change rate
	gl_TexCoord[1].st = gl_TexCoord[0].st;
	gl_TexCoord[1].p = currentTime*0.2 + genattrib1.w;

	// set the final position & transform	
	vec4 new_vertex = gl_ModelViewMatrix * vec4(position,1.0);
	new_vertex.xy += particleSize*(1.0 + 0.5*genattrib2.x)*gl_Vertex.xy;
	gl_Position = gl_ProjectionMatrix * new_vertex;
	
	normal = vec3(0.0,0.0,1.0);		// make the basic normal in view space
	normal += gl_Vertex.xyz;		// spread it
	
	viewVec = new_vertex.xyz;
	lightVec = normalize((gl_ModelViewMatrix * vec4(lightPosition.xyz,0.0)).xyz);
}