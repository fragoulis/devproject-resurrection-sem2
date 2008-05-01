uniform float distortionFreq;
uniform float distortionScale;
uniform float distortionRoll;
uniform float interference;
uniform float time_0_X;


uniform sampler2D Image;
uniform sampler3D Noise;
uniform sampler3D Rand;

varying vec2 vPos;

void main(void)
{
   
   // Interference ... just a texture filled with rand()
   float rand = float(texture3D(Rand, vec3(1.5 * vPos, time_0_X))) - 0.2;
   
   // Some signed noise for the distortion effect
   float noisy = float(texture3D(Noise, vec3(0.0, 0.5 * vPos.y, 0.1 * time_0_X))) - 0.5;
   
   // Repeat a 1 - x^2 (0 < x < 1) curve and roll it with sinus.
   float dst = fract(vPos.y * distortionFreq + distortionRoll);
   dst *= (1.0 - dst);
   
   // Make sure distortion is highest in the center of the image
   dst /= 1.0 + distortionScale * abs(vPos.y);
   
   // ... and finally distort
   vec2 texCoord = gl_TexCoord[0].xy;
   //texCoord.x +=  /*distortionScale */ noisy * dst;
   vec4 image = texture2D(Image, texCoord.xy);
   
   //if (image.r <= 0.1 && image.g <= 0.1 && image.b <= 0.1)
     // discard;
     
   float discardthis = step(image.r,0.1)*step(image.b,0.1)*step(image.g,0.1);
   //if (discardthis > 0.5)
      //discard;
   
   
   // Combine frame, distorted image and interference
   gl_FragColor = (interference * rand + image);
   
   
   
}