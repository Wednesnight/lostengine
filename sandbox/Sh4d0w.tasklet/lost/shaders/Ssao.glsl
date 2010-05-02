return Shader:declare
{
  export =
  {
    vertex =
    [[
      void processSsao(void);
    ]],

    fragment =
    [[
      vec4 processSsao(in vec4 color, in sampler2D tex);
    ]]
  },

  vertex = Shader:source
  [[
    // constant matrix used to scale texture coordinates
    const mat4 biasMatrix = mat4(vec4(0.5, 0.0, 0.0, 0.0),
                                 vec4(0.0, 0.5, 0.0, 0.0),
                                 vec4(0.0, 0.0, 0.5, 0.0),
                                 vec4(0.5, 0.5, 0.5, 1.0));

    // mesh transform
    uniform mat4 modelViewMatrix;

    // from camera
    uniform mat4 projectionMatrix;

    attribute vec3 position;

    varying vec4 ssaoTexcoord;

    void processSsao(void)
    {
      ssaoTexcoord = biasMatrix * projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
  ]],

  fragment = Shader:source
  [[
    uniform vec2 depth;
    uniform vec2 viewport;

    varying vec4 ssaoTexcoord;

    float readDepth(in vec2 coord, in sampler2D tex)
    {
    	return (2.0 * depth.x) / (depth.y + depth.x - texture2D(tex, coord).x * (depth.y - depth.x));	
    }

    float compareDepths(in float depth1, in float depth2)
    {
    	float aoCap = 1.0;
    	float aoMultiplier=10000.0;
    	float depthTolerance=0.000;
    	float aorange = 10.0;// units in space the AO effect extends to (this gets divided by the camera far range
    	float diff = sqrt( clamp(1.0-(depth1-depth2) / (aorange/(depth.y-depth.x)),0.0,1.0) );
    	float ao = min(aoCap,max(0.0,depth1-depth2-depthTolerance) * aoMultiplier) * diff;
    	return ao;
    }

    vec4 processSsao(in vec4 color, in sampler2D tex)
    {	
    	float ao = 0.0;
      vec2 texcoord = ssaoTexcoord.xy / ssaoTexcoord.w;

    	float ssaoDepth = readDepth(texcoord, tex);
    	float d;

    	float pw = 1.0 / viewport.x;
    	float ph = 1.0 / viewport.y;
    	float aoscale = 1.0;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	pw *= 1.2;
    	ph *= 1.2;
    	aoscale *= 1.2;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	pw *= 1.2;
    	ph *= 1.2;
    	aoscale *= 1.2;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	pw *= 1.2;
    	ph *= 1.2;
    	aoscale *= 1.2;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y+ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x+pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	d=readDepth( vec2(texcoord.x-pw,texcoord.y-ph), tex);
    	ao+=compareDepths(ssaoDepth,d)/aoscale;

    	ao /= 16.0;

      return vec4((1.0 - ao) * color.rgb, color.a);
    }
  ]]
}
