uniform vec2 depth;
uniform vec2 viewport;

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

float calculateSsaoFactor(in vec4 ssaoTexcoord, in sampler2D tex)
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

  return ao;
}
