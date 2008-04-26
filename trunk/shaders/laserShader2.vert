// In texcoord 0 .pq we'll store the flare tex coords

void main(void)
{
   gl_Position = ftransform();
   gl_TexCoord[0].pq = gl_MultiTexCoord0.ts;
   gl_TexCoord[0].st = 2.0*(gl_TexCoord[0].pq-vec2(0.5));

   
}