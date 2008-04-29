varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,viewVec;

uniform sampler2D texmap0;


void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = ambient;
	
	vec4 texcolor = texture2D(texmap0,gl_TexCoord[0].st);
	
	n = normalize(normal);
	
	NdotL = max(dot(n,lightDir),0.0);

	if (NdotL > 0.0) {
		vec3 V = normalize(viewVec);
		halfV = reflect(lightDir,n);
		NdotHV = max(dot(halfV,V),0.0);
		color += diffuse * NdotL;
		color *= texcolor;
		color += gl_FrontMaterial.specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}

	gl_FragColor = color;
}