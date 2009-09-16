require("Application")

Field =
{
  player = -1
}

Field_mt = { __index = Field }
function Field()
  local result = {}
  setmetatable(result, Field_mt)
  return result
end

TixTixTix =
{
  combinationLookup =
  {
    [0] = { 0,3,6 },
    [1] = { 0,4 },
    [2] = { 0,5,7 },
    [3] = { 1,3 },
    [4] = { 1,4,6,7 },
    [5] = { 1,5 },
    [6] = { 2,3,7 },
    [7] = { 2,4 },
    [8] = { 2,5,6 }
  },

  dispatcher = nil,

  validCombinations =
  {
    [0] = { 0,1,2 },
    [1] = { 3,4,5 },
    [2] = { 6,7,8 },
    [3] = { 0,3,6 },
    [4] = { 1,4,7 },
    [5] = { 2,5,8 },
    [6] = { 0,4,8 },
    [7] = { 2,4,6 }
  }
}

function TixTixTix:init(eventDispatcher)
  self:resetGame()

  self.renderState = lost.application.Application.context:copyState()
  self.renderState.clearColor = lost.common.Color(0,0,0)
  self.renderState.depthTest = false
  self.renderState.texture2D = false
  self.renderState.vertexArray = true

  dispatcher = eventDispatcher
  dispatcher:addEventListener(lost.application.ResizeEvent.MAIN_WINDOW_RESIZE, function(event) self.resizeHandler(self, event) end)

  if lost.platform.isIPhone() then
    dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_BEGAN, function(event) self.touchHandler(self, event) end)
    dispatcher:addEventListener(lost.application.TouchEvent.TOUCHES_ENDED, function(event) self.touchHandler(self, event) end)
  else
    dispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event) self.keyHandler(self, event) end)
    dispatcher:addEventListener(lost.application.KeyEvent.KEY_UP, function(event) self.keyHandler(self, event) end)
    dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, function(event) self.clickHandler(self, event) end)
    dispatcher:addEventListener(lost.application.MouseEvent.MOUSE_UP, function(event) self.clickHandler(self, event) end)
  end

  self.timer = lost.application.Timer("TixTixTixRender", 1/30)
  self.timer:addEventListener(lost.application.TimerEvent.TIMER_FIRED, function(event) self.renderHandler(self, event) end)
end

function TixTixTix:resetGame()
  log.debug("resetting game")
  self.currentPlayer = 0
  self.board =
    {
      [0] =
      {
        [0] = Field(),
        [1] = Field(),
        [2] = Field()
      },
      [1] =
      {
        [0] = Field(),
        [1] = Field(),
        [2] = Field()
      },
      [2] =
      {
        [0] = Field(),
        [1] = Field(),
        [2] = Field()
      }
    }
end

function TixTixTix:keyHandler(event)
  local keyEvent = lost.application.KeyEvent.cast(event)

  if (keyEvent.pressed) then
  
    if (keyEvent.key == lost.application.K_ESCAPE) then
      globals.app:quit()
    end
  
  end
end

function TixTixTix:clickField(location)
  local valid = false

  local x = math.floor(location.x / lost.application.Application.config.FieldSize.x)
  local y = math.floor(location.y / lost.application.Application.config.FieldSize.y)
  valid = (x >= 0 and x <= 2) and (y >= 0 and y <= 2)
  if valid then
    if self.board[x][y].player == -1 then
      self.board[x][y].player = self.currentPlayer
      valid = true
    end
  end

  if valid then
    if self.currentPlayer == 0 then
      self.currentPlayer = 1
    else
      self.currentPlayer = 0
    end
  end
end

function TixTixTix:touchHandler(event)
  local touchEvent = lost.application.TouchEvent.cast(event)

  if (event.type == lost.application.TouchEvent.TOUCHES_BEGAN) then
    if (touchEvent:size() == 1) then
      local touch = touchEvent:get(0)
      local location = touch.location
      location.y = (lost.application.Application.config.displayAttributes.height - location.y)
      self:clickField(location)
    end
  elseif (event.type == lost.application.TouchEvent.TOUCHES_ENDED and touchEvent:size() == 4) then
    self:resetGame()
  end
end

function TixTixTix:clickHandler(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)

  if (event.type == lost.application.MouseEvent.MOUSE_DOWN) and (mouseEvent.button == lost.application.MB_LEFT) then
    self.leftButtonDown = true
  elseif (event.type == lost.application.MouseEvent.MOUSE_UP) and (mouseEvent.button == lost.application.MB_LEFT) then
    self.leftButtonDown = false
  end
  if (self.leftButtonDown) and (event.type == lost.application.MouseEvent.MOUSE_UP) and (mouseEvent.button == lost.application.MB_RIGHT) then
    self:resetGame()
  else
    if (event.type == lost.application.MouseEvent.MOUSE_DOWN) then
      self:clickField(mouseEvent.pos)
    end
  end
end

function TixTixTix:renderHandler(event)
  local Application = lost.application.Application
  local timerEvent = lost.application.TimerEvent.cast(event)

  Application.context:pushState(self.renderState)
  Application.context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
  Application.context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(Application.config.displayAttributes.width, Application.config.displayAttributes.height))

  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()

  gl.glLineWidth(1.0)
  for x = 0, 2 do
    for y = 0, 2 do
      lost.gl.setColor(lost.common.Color(1,1,1))
      lost.gl.drawRectOutline(lost.math.Rect(x*Application.config.FieldSize.x, y*Application.config.FieldSize.y, Application.config.FieldSize.x, Application.config.FieldSize.y))
      if self.board[x][y].player == 0 then
        lost.gl.setColor(lost.common.Color(1,0,0))
        lost.gl.drawRectFilled(lost.math.Rect(x*Application.config.FieldSize.x+1, y*Application.config.FieldSize.y+1, Application.config.FieldSize.x-1, Application.config.FieldSize.y-1))
      elseif self.board[x][y].player == 1 then
        lost.gl.setColor(lost.common.Color(0,0,1))
        lost.gl.drawRectFilled(lost.math.Rect(x*Application.config.FieldSize.x+1, y*Application.config.FieldSize.y+1, Application.config.FieldSize.x-1, Application.config.FieldSize.y-1))
      end
    end
  end

  Application.context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(Application.config.displayAttributes.width, Application.config.displayAttributes.height))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()
  controls.fpsMeter:render(Application.config.displayAttributes.width - controls.fpsMeter.width, 0, timerEvent.passedSec)

  Application.context:popState()
  globals.app:swapBuffers()
end

function TixTixTix:resizeHandler(event)
  local resizeEvent = lost.application.ResizeEvent.cast(event)

  lost.application.Application.config.FieldSize = lost.math.Vec2((resizeEvent.width / 3), (resizeEvent.height / 3))
  gl.glViewport(0, 0, resizeEvent.width, resizeEvent.height) gl.GLDEBUG()
end

TixTixTix_mt = { __index = TixTixTix }
function TixTixTix(eventDispatcher)
  local result = {}
  setmetatable(result, TixTixTix_mt)
  result:init(eventDispatcher)
  return result
end
