uniform vec4 lightPosition;
uniform vec4 lightColor;
varying vec4 diffuse,ambient;
varying vec3 normal,lightDir,viewVec;

void main()
{	
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize((gl_ModelViewMatrix * vec4(lightPosition.xyz,0.0)).xyz);
	
	diffuse = gl_FrontMaterial.diffuse * lightColor;
	ambient = 0.1 * lightColor * gl_FrontMaterial.ambient;
	
	viewVec = (gl_ModelViewMatrix * gl_Vertex).xyz;
		
	gl_Position = ftransform();	
} 
