//
//  Shader.fsh
//  gltest
//
//  Created by Tony Kostanjsek on 19.02.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
	gl_FragColor = colorVarying;
}
