
return 
dcl.rg:Node
{
  dcl.rg:ClearColor { color = Color(92/255,174/255,210/255,1) },
  dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT },
  dcl.rg:DepthTest{false},
  dcl.rg:Camera2D { viewport = Rect(0,0,config.window.width, config.window.height) },    

  meshFactory:createComboRectNode(gray2, gray1, Rect(left,390,200,200), 27, 2),
  meshFactory:iqcreateComboRectNode(gray2, gray1, Rect(right,390,200,200), 27, 2),  
}