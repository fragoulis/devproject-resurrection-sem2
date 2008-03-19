uniform sampler3D noiseTex;
uniform sampler2D shadowTex;
uniform sampler2D cloudTex;
uniform sampler2D heightTex;

uniform vec4 lightColor;

varying vec3 viewVec;
varying vec3 lightVec;
varying vec3 normal;

const vec4 lakeColor = vec4(0.5,0.9,1.0,1.0);

const vec3 wavedir = normalize(vec3(-lightVec.xy,0.0));

void main()
{
	vec3 V = normalize(viewVec);
	float bump = texture3D(noiseTex,gl_TexCoord[0].stp).r*2.0;
	vec3 n = normalize(normal + wavedir*bump);
	
	float shadowContrib = texture2DProj(shadowTex, gl_TexCoord[1]).r;
	
	// Compute the new .st for the clouds
	vec4 cloudColor = texture2D(cloudTex,gl_TexCoord[2].st);
	
	// compute lighting
	float NdotL = clamp(dot(n,lightVec),0.0,1.0);
	vec4 Is = vec4(0.0);
	if(NdotL > 0.0)
	{
		vec3 R = normalize(reflect(lightVec, n));
		Is = vec4(pow(max(dot(R, V), 0.0), 5.0))*step(shadowContrib,0.1);
	}
	
	// get height
	float height = texture2D(heightTex,gl_TexCoord[3].st).r;
	
	vec4 color = (1.0 - shadowContrib)*NdotL*(lightColor + 0.0001*cloudColor)*lakeColor + Is;
	color.a = 1.0 - smoothstep(-15.0,1.0,height);
	gl_FragColor = color;
}