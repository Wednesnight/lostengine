
return 
dcl.rg:Node
{
  dcl.rg:ClearColor { color = Color(1,1,1,1) },
  dcl.rg:Clear { mask = gl.GL_COLOR_BUFFER_BIT },
  dcl.rg:DepthTest{false},
  dcl.rg:Camera2D { viewport = Rect(0,0,config.window.width, config.window.height) },    
--  meshFactory:comboRectNode(blue, red, Rect(left,390,200,200), 27, 2, {tl=false, tr=true, bl=false, br=true}, {left=false}),
--  meshFactory:iqcreateComboRectNode(yellow, white, Rect(right,390,200,200), 27, 2),  
}