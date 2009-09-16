// This texture should hold the image to blur.
// This can perhaps be a previous rendered scene.
uniform sampler2D colorMap;
 
// some const, tweak for best look
const float sampleDist = 0.5;
const float sampleStrength = 4.0; 
 
void main(void)
{
   // texture coordinates
   vec2 uv = vec2(gl_TexCoord[0]);
 
   // some sample positions
   float samples[10];
   samples[0] = -0.08;
   samples[1] = -0.05;
   samples[2] = -0.03;
   samples[3] = -0.02;
   samples[4] = -0.01;
   samples[5] =  0.01;
   samples[6] =  0.02;
   samples[7] =  0.03;
   samples[8] =  0.05;
   samples[9] =  0.08;
 
    // 0.5,0.5 is the center of the screen
    // so substracting uv from it will result in
    // a vector pointing to the middle of the screen
    vec2 dir = 0.5 - uv; 
 
    // calculate the distance to the center of the screen
    float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
 
    // normalize the direction (reuse the distance)
    dir = dir/dist; 
 
    // this is the original colour of this fragment
    // using only this would result in a nonblurred version
    vec4 color = texture2D(colorMap,uv); 
 
    vec4 sum = color;
 
    // take 10 additional blur samples in the direction towards
    // the center of the screen
    for (int i = 0; i < 10; i++)
    {
      sum += texture2D( colorMap, uv + dir * samples[i] * sampleDist );
    }
 
    // we have taken eleven samples
    sum *= 1.0/11.0;
 
    // weighten the blur effect with the distance to the
    // center of the screen ( further out is blurred more)
    float t = dist * sampleStrength;
    t = clamp( t ,0.0,1.0); //0 &lt;= t &lt;= 1
 
    //Blend the original color with the averaged pixels
    gl_FragColor = mix( color, sum, t );
}
