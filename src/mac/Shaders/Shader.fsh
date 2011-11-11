//
//  Shader.fsh
//  lw
//
//  Created by Li Wei on 11-4-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
