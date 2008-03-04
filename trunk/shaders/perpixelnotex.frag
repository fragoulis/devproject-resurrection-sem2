varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,halfVector;


void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = ambient;
	
	n = normalize(normal);
	
	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		halfV = normalize(halfVector);
		NdotHV = max(dot(n,halfV),0.0);
		color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
		color += diffuse * NdotL;
	}

	gl_FragColor = color;
}