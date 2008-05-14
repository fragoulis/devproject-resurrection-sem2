uniform samplerRect heightmap;
uniform vec3 xyzscale;
//uniform float dimdenom;


void main()
{
	// Get height
	float height = textureRect(heightmap,gl_TexCoord[0].st).r;
	float hl  = textureRect(heightmap, gl_TexCoord[0].st + vec2(-1.0,0.0)).r;
    float ht  = textureRect(heightmap, gl_TexCoord[0].st + vec2(0.0, 1.0)).r;
    float hr  = textureRect(heightmap, gl_TexCoord[0].st + vec2( 1.0,0.0)).r;
    float hb  = textureRect(heightmap, gl_TexCoord[0].st + vec2(0.0,-1.0)).r;
    float htl  = textureRect(heightmap, gl_TexCoord[0].st + vec2(-1.0,1.0)).r;
    float htr  = textureRect(heightmap, gl_TexCoord[0].st + vec2(1.0, 1.0)).r;
    float hbl  = textureRect(heightmap, gl_TexCoord[0].st + vec2( -1.0,-1.0)).r;
    float hbr  = textureRect(heightmap, gl_TexCoord[0].st + vec2(1.0,-1.0)).r;
	
	// compute position
	vec4 pos = vec4(1.0);
	pos.y = height * xyzscale.y;
	pos.xz = gl_TexCoord[0].st * xyzscale.xz;
	pos.z *= -1.0;
	
	// compute normal 
	vec3 c =  xyzscale*vec3(gl_TexCoord[0].s,height,-gl_TexCoord[0].t);
    vec3 l  = xyzscale*vec3(gl_TexCoord[0].s-1.0,hl,-gl_TexCoord[0].t) - c;
    vec3 t  = xyzscale*vec3(gl_TexCoord[0].s,ht,-(gl_TexCoord[0].t+1.0)) - c;
    vec3 r  = xyzscale*vec3(gl_TexCoord[0].s+1.0,hr,-gl_TexCoord[0].t) - c;
    vec3 b  = xyzscale*vec3(gl_TexCoord[0].s,hb,-(gl_TexCoord[0].t-1.0)) - c;
    vec3 tl  = xyzscale*vec3(gl_TexCoord[0].s-1.0,htl,-(gl_TexCoord[0].t+1.0)) - c;
    vec3 tr  = xyzscale*vec3(gl_TexCoord[0].s+1.0,htr,-(gl_TexCoord[0].t+1.0)) - c;
    vec3 bl  = xyzscale*vec3(gl_TexCoord[0].s-1.0,hbl,-(gl_TexCoord[0].t-1.0)) - c;
    vec3 br  = xyzscale*vec3(gl_TexCoord[0].s+1.0,hbr,-(gl_TexCoord[0].t-1.0)) - c;
    
	gl_FragData[0] = pos;
	gl_FragData[1] = vec4(normalize(cross(t,tl) + 
									cross(tl,l) + 
									cross(l,bl) + 
									cross(bl,b) + 
									cross(b,br) + 
									cross(br,r) + 
									cross(r,tr)+
									cross(tr,t)),1.0);
}