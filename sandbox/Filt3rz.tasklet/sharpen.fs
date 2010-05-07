#define KERNEL_SIZE 9

// Gaussian kernel
// 1 2 1
// 2 4 2
// 1 2 1	
float kernel[KERNEL_SIZE];

uniform sampler2D texture0;
uniform vec2 viewport;

varying vec2 texcoord;

float step_w = 1.0/viewport.x;
float step_h = 1.0/viewport.y;

vec2 offset[KERNEL_SIZE];
						 
void main(void)
{
   int i = 0;
   vec4 sum = vec4(0.0);
   
   offset[0] = vec2(-step_w, -step_h);
   offset[1] = vec2(0.0, -step_h);
   offset[2] = vec2(step_w, -step_h);
   
   offset[3] = vec2(-step_w, 0.0);
   offset[4] = vec2(0.0, 0.0);
   offset[5] = vec2(step_w, 0.0);
   
   offset[6] = vec2(-step_w, step_h);
   offset[7] = vec2(0.0, step_h);
   offset[8] = vec2(step_w, step_h);
   
   kernel[0] = -1.0; 	kernel[1] = -1.0;	kernel[2] = -1.0;
   kernel[3] = -1.0;	kernel[4] = 9.0;	kernel[5] = -1.0;
   kernel[6] = -1.0;   	kernel[7] = -1.0;	kernel[8] = -1.0;
   
   
   for( i=0; i<KERNEL_SIZE; i++ )
   {
    vec4 tmp = texture2D(texture0, texcoord.st + offset[i]);
    sum += tmp * kernel[i];
   }
   gl_FragColor = sum;
}