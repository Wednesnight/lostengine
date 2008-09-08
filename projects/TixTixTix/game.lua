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

local game =
{
  currentPlayer = 0,
  board =
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
}

local validCombinations =
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

local combinationLookup =
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
}

function keyHandler(event)
  local keyEvent = lost.application.KeyEvent.cast(event)

  if (keyEvent.pressed) then
  
    if (keyEvent.key == lost.application.K_ESCAPE) then
      globals.app:quit()
    end
  
  end
end

function clickField(location)
  local valid = false

  local x = math.floor(location.x / Application.config.FieldSize.x)
  local y = math.floor(location.y / Application.config.FieldSize.y)
  if game.board[x][y].player == -1 then
    game.board[x][y].player = game.currentPlayer
    valid = true
  end

  if valid then
    if game.currentPlayer == 0 then
      game.currentPlayer = 1
    else
      game.currentPlayer = 0
    end
  end
end

function touchHandler(event)
  local touchEvent = lost.application.TouchEvent.cast(event)

  if (event.type == lost.application.TouchEvent.TOUCHES_BEGAN) then
    if (touchEvent:size() == 1) then
      local touch = touchEvent:get(0)
      local location = touch.location
      location.y = (Application.config.displayAttributes.height - location.y)
      clickField(location)
    end
  end
end

function clickHandler(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)

  if (event.type == lost.application.MouseEvent.MOUSE_DOWN) then
    clickField(mouseEvent.pos)
  end
end

function renderHandler(event)
  local timerEvent = lost.application.TimerEvent.cast(event)

  gl.glClearColor(0.0, 0.0, 0.0, 0.0) gl.GLDEBUG()
  gl.glClear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT) gl.GLDEBUG()

  gl.glEnableClientState(gl.GL_VERTEX_ARRAY) gl.GLDEBUG()
  gl.glDisable(gl.GL_DEPTH_TEST) gl.GLDEBUG()
  gl.glDisable(gl.GL_TEXTURE_2D) gl.GLDEBUG()

  lost.gl.utils.set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(globals.app.displayAttributes.width, globals.app.displayAttributes.height))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()

  for x = 0, 2 do
    for y = 0, 2 do
      if game.board[x][y].player == 0 then
        lost.gl.setColor(lost.common.Color(1,0,0))
        lost.gl.drawRectFilled(lost.math.Rect(x*Application.config.FieldSize.x+1, y*Application.config.FieldSize.y+1, Application.config.FieldSize.x-1, Application.config.FieldSize.y-1))
      elseif game.board[x][y].player == 1 then
        lost.gl.setColor(lost.common.Color(0,0,1))
        lost.gl.drawRectFilled(lost.math.Rect(x*Application.config.FieldSize.x+1, y*Application.config.FieldSize.y+1, Application.config.FieldSize.x-1, Application.config.FieldSize.y-1))
      else
        lost.gl.setColor(lost.common.Color(1,1,1))
        lost.gl.drawRectOutline(lost.math.Rect(x*Application.config.FieldSize.x+1, y*Application.config.FieldSize.y+1, Application.config.FieldSize.x-1, Application.config.FieldSize.y-1))
      end
    end
  end

  lost.gl.utils.set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(globals.app.displayAttributes.width, globals.app.displayAttributes.height))
  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()
  controls.fpsMeter:render(globals.app.displayAttributes.width - controls.fpsMeter.width, 0, timerEvent.passedSec)

  gl.glDisableClientState(gl.GL_VERTEX_ARRAY) gl.GLDEBUG()
  
  globals.app:swapBuffers()
end

function resizeHandler(event)
  local resizeEvent = lost.application.ResizeEvent.cast(event)

  gl.glViewport(0, 0, resizeEvent.width, resizeEvent.height) gl.GLDEBUG()
end
