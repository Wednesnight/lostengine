//=========================================================================
// Copyright (C) Tommy Hinks						  
// tommy[dot]hinks[at]gmail[dot]com  												  
//
// Contributors: 
//             1) Tommy Hinks
//=========================================================================

uniform float line_width;
uniform float line_exp;
uniform vec4 line_color;

void main()
{
	// Find smallest distance to triangle edge.
	// Post-multiply distances to avoid perspective correction from interpolation.
	//
	float min_d = 
		min(gl_TexCoord[0].x*gl_TexCoord[0].w, 
			min(gl_TexCoord[0].y*gl_TexCoord[0].w, gl_TexCoord[0].z*gl_TexCoord[0].w));
	
	// Compare fragment closest distance to edge with
	// desired line width.
	//
	float alpha = min_d - 0.5*line_width + 1.0;
	
	if( alpha < 0.0 )
	{	// Within line width.
		//
		gl_FragColor = line_color;
	}
	else if( 0.0 < alpha && alpha < 2.0 )
	{	// On line boundary, fade between line color and triangle color (i.e. gl_Color).
		//
		float t = exp2(-(line_exp*alpha*alpha));
		gl_FragColor = mix(gl_Color, line_color, t);
	}
	else
	{	// Outside line.
		//
		gl_FragColor = gl_Color;
	}
}