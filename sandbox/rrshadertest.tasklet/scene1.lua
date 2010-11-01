
return 
dcl.rg:Node
{
  dcl.rg:ClearColor { color = Color(92/255,174/255,210/255,1) },
  dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT },
  dcl.rg:DepthTest{false},
  dcl.rg:Camera2D { viewport = Rect(0,0,config.window.width, config.window.height) },    

  meshFactory:createComboRectNode(gray2, gray1, Rect(left,390,200,200), 27, 2),
  meshFactory:iqcreateComboRectNode(gray2, gray1, Rect(right,390,200,200), 27, 2),
  
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(20,180, 10, 10), r,w) },
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(20,20, 16, 16), r,w) },
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(100,50, 64, 64), r,w) },
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(10,100, 13, 13), r,w) },
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(200,200, 57, 57), r,w) },
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(420,200, 64, 46), r,w) },

  meshFactory:createComboRectNode(gray2, gray1, Rect(left, 350, 120, 20), r, 1),
  meshFactory:iqcreateComboRectNode(gray2, gray1, Rect(right, 350, 120, 20), r, 1),
  
  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(left,230, 100, 100), r,w) },
  dcl.rg:Draw { mesh = meshFactory:iqcreateRoundedRectFrame(white, Rect(right,230, 100, 100), r,w) },

  dcl.rg:Draw { mesh = meshFactory:createRoundedRectFrame(white, Rect(left,30, 100, 100), r,1) },
  dcl.rg:Draw { mesh = meshFactory:iqcreateRoundedRectFrame(white, Rect(right, 30, 100, 100), r,1) },

  dcl.rg:Draw { mesh = meshFactory:createDisc(white, Vec2(600,300), 47) },

  dcl.rg:Draw { mesh = meshFactory:iqcreateRoundedRectFrame(white, Rect(right,180, 10, 10), r,w) },

  dcl.rg:Draw { mesh = meshFactory:createBox(white, Rect(right,10, 400, 400)) },
}