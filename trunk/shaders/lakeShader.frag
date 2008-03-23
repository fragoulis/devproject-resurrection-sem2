uniform sampler3D noiseTex;
uniform sampler2D shadowTex;
uniform sampler2D reflTex;
uniform sampler2D heightTex;

uniform vec4 lightColor;

varying vec3 viewVec;
varying vec3 lightVec;
varying vec3 normal;
varying mat3 tbn;

uniform vec4 waterColor;

void main()
{
	vec3 V = normalize(viewVec);
	
	float bump = texture3D(noiseTex,gl_TexCoord[0].stp).r;
	bump = bump * 2.0 - 1.0;
	vec3 n = normalize(normal + -V*bump);
	
	//vec3 n = normalize(tbn * (texture2D(noiseTex,gl_TexCoord[0].st).rgb*2.0 - 1.0));
	
	float shadowContrib = texture2D(shadowTex, gl_TexCoord[1].st).r;
	
	// Compute the new .st for the clouds
	vec3 refl = reflect(V,n);
	vec3 offset = V + refl;
	vec2 current_tex_pos = gl_TexCoord[1].st;
	vec2 center_tex_pos = vec2(0.5);								//central tex coord on screen
	vec2 diff = current_tex_pos - center_tex_pos;					// texcoord difference vector
	vec2 dist_pcent = clamp(vec2(0.5) - abs(diff),vec2(0.0),vec2(0.5));	// 0.0 -> edge , 0.5 -> center
	vec2 actual_coords = gl_TexCoord[1].st + 2.0*dist_pcent*offset.xy*0.2;
	
	vec4 reflColor = texture2D(reflTex,actual_coords);
	
	// compute lighting
	float NdotL = clamp(dot(n,lightVec),0.0,1.0);
	vec4 Is = vec4(0.0);
	if(NdotL > 0.0)
	{
		vec3 R = normalize(reflect(lightVec, n));
		Is = lightColor*(pow(max(dot(R, V), 0.0), 128.0))*step(shadowContrib,0.1);
	}
	
	// get height
	float height = texture2D(heightTex,gl_TexCoord[3].st).r;
	
	vec4 color = (1.0 - shadowContrib)*NdotL*lightColor*waterColor + Is;
	color.a = 1.0 - smoothstep(-15.0,1.0,height);
	
	float reflectionContrib = 1.0 - reflColor.a*0.6;
	color.rgb = mix(reflColor.rgb,color.rgb,reflectionContrib);
	gl_FragColor = color;
}