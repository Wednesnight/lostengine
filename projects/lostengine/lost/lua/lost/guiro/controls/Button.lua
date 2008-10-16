--[[
     Button class
  ]]
class 'Button' (lost.guiro.View)

function Button:__init() super()
  log.debug("Button:__init()")
end

function Button:render()
--  log.debug("Button:render()")
  
  lost.gl.setColor(lost.common.Color(0,1,0))
  lost.gl.drawRectFilled(lost.math.Rect(100,100,100,100))
end

function Button:__finalize()
  log.debug("Button:__finalize()")
end
