//=========================================================================
// Copyright (C) Tommy Hinks						  
// tommy[dot]hinks[at]gmail[dot]com  												  
//
// Contributors: 
//             1) Tommy Hinks
//=========================================================================

uniform vec4 viewport;

void main()
{	// Light direction is always (0.0, 0.0, 1.0), i.e. looking out of the camera.
	// This means that diffuse shading depends only on the z-coordinate of the transformed
	// normal in eye-space.
	//
	const vec3 normal = normalize(gl_NormalMatrix*gl_Normal);

	// Compute diffuse and ambient only, no specular.
	// Vertices of the same triangle have the same normal, 
	// hence lighting is done per triangle. Lighting and material 
	// properties are passed through built-in uniforms.
	//
	gl_FrontColor = 
		max(normal.z, 0.0)*(gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse) + 
		gl_FrontMaterial.ambient*gl_LightSource[0].ambient;

	// Stored in the (multi-)texture coordinates for each vertex are the 
	// three vertex positions of the triangle that the vertex belongs to.
	// Project the triangle vertices into viewport (screen) space.
	//
	vec4 vp0 = gl_ModelViewProjectionMatrix*vec4(gl_MultiTexCoord0.xyz, 1.0);
	vp0.x /= vp0.w;
	vp0.y /= vp0.w;
	vp0.z /= vp0.w;
	vp0.x = viewport.x + 0.5*viewport.z*(vp0.x + 1.0);
	vp0.y = viewport.y + 0.5*viewport.w*(vp0.y + 1.0);
	vp0.z = gl_DepthRange.near + 0.5*gl_DepthRange.far*(vp0.z + gl_DepthRange.diff);
	
	vec4 vp1 = gl_ModelViewProjectionMatrix*vec4(gl_MultiTexCoord1.xyz, 1.0);
	vp1.x /= vp1.w;
	vp1.y /= vp1.w;
	vp1.z /= vp1.w;
	vp1.x = viewport.x + 0.5*viewport.z*(vp1.x + 1.0);
	vp1.y = viewport.y + 0.5*viewport.w*(vp1.y + 1.0);
	vp1.z = gl_DepthRange.near + 0.5*gl_DepthRange.far*(vp1.z + gl_DepthRange.diff);

	vec4 vp2 = gl_ModelViewProjectionMatrix*vec4(gl_MultiTexCoord2.xyz, 1.0);
	vp2.x /= vp2.w;
	vp2.y /= vp2.w;
	vp2.z /= vp2.w;
	vp2.x = viewport.x + 0.5*viewport.z*(vp2.x + 1.0);
	vp2.y = viewport.y + 0.5*viewport.w*(vp2.y + 1.0);
	vp2.z = gl_DepthRange.near + 0.5*gl_DepthRange.far*(vp2.z + gl_DepthRange.diff);

	// Project current vertex into viewport (screen) space.
	//
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	vec4 vp = gl_Position;
	vp.x /= vp.w;
	vp.y /= vp.w;
	vp.z /= vp.w;
	vp.x = viewport.x + 0.5*viewport.z*(vp.x + 1.0);
	vp.y = viewport.y + 0.5*viewport.w*(vp.y + 1.0);
	vp.z = gl_DepthRange.near + 0.5*gl_DepthRange.far*(vp.z + gl_DepthRange.diff);

	// Compute the distance in viewport (screen) space to each of the triangle
	// edges. These distances are pre-multiplied by the perspective component
	// of the vertex to avoid errors with perspective correction. The inverse
	// perspective component is interpolated and used in the fragment shader (gl_TexCoord[0].w).
	//
	float dx, dy;
	dx = vp1.x - vp0.x;
	dy = vp1.y - vp0.y;
	gl_TexCoord[0].x = abs(dx*(vp0.y - vp.y) - (vp0.x - vp.x)*dy)/sqrt(dx*dx + dy*dy);
	gl_TexCoord[0].x *= vp.w;	// Perspective pre-multiplication.

	dx = vp2.x - vp1.x;
	dy = vp2.y - vp1.y;
	gl_TexCoord[0].y = abs(dx*(vp1.y - vp.y) - (vp1.x - vp.x)*dy)/sqrt(dx*dx + dy*dy);
	gl_TexCoord[0].y *= vp.w;	// Perspective pre-multiplication.

	dx = vp0.x - vp2.x;
	dy = vp0.y - vp2.y;
	gl_TexCoord[0].z = abs(dx*(vp2.y - vp.y) - (vp2.x - vp.x)*dy)/sqrt(dx*dx + dy*dy);
	gl_TexCoord[0].z *= vp.w;	// Perspective pre-multiplication.
	
	gl_TexCoord[0].w = 1.0/vp.w;	// Varying post-multiplication for fragment shader.
}