varying vec2 vPos;

void main(void)
{
   vec2 Pos = ftransform(); //sign(gl_Vertex.xy);
   
   gl_Position = vec4(Pos.xy, 0.0, 1.0);
   //gl_TexCoord[0].xy = Pos; // * 0.5 + 0.5;     
   //vPos = Pos;
   
   gl_TexCoord[0].xy = gl_MultiTexCoord0.st;
   vPos = 2.0* ( gl_TexCoord[0].xy - vec2 ( 0.5) );
   
   //gl_TexCoord[0].st = gl_MultiTexCoord0.st;
   //gl_Position = ftransform();
}