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

function resetGame()
  log.debug("resetting game")
  game =
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
end

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

  local x = math.floor(location.x / lost.application.Application.config.FieldSize.x)
  local y = math.floor(location.y / lost.application.Application.config.FieldSize.y)
  valid = (x >= 0 and x <= 2) and (y >= 0 and y <= 2)
  if valid then
    if game.board[x][y].player == -1 then
      game.board[x][y].player = game.currentPlayer
      valid = true
    end
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
      location.y = (lost.application.Application.config.displayAttributes.height - location.y)
      clickField(location)
    end
  elseif (event.type == lost.application.TouchEvent.TOUCHES_ENDED and touchEvent:size() == 4) then
    resetGame()
  end
end

function clickHandler(event)
  local mouseEvent = lost.application.MouseEvent.cast(event)

  if (event.type == lost.application.MouseEvent.MOUSE_DOWN) and (mouseEvent.button == lost.application.MB_LEFT) then
    game.leftButtonDown = true
  elseif (event.type == lost.application.MouseEvent.MOUSE_UP) and (mouseEvent.button == lost.application.MB_LEFT) then
    game.leftButtonDown = false
  end
  if (game.leftButtonDown) and (event.type == lost.application.MouseEvent.MOUSE_UP) and (mouseEvent.button == lost.application.MB_RIGHT) then
    resetGame()
  else
    if (event.type == lost.application.MouseEvent.MOUSE_DOWN) then
      clickField(mouseEvent.pos)
    end
  end
end

function renderHandler(event)
  local Application = lost.application.Application
  local timerEvent = lost.application.TimerEvent.cast(event)

  local newState = Application.context:copyState()
  newState.vertexArray = true
  newState.depthTest = false
  newState.texture2D = false
  newState.clearColor = lost.common.Color(0.0, 0.0, 0.0, 0.0)
  Application.context:pushState(newState)

  Application.context:clear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT)
  Application.context:set2DProjection(lost.math.Vec2(0,0), lost.math.Vec2(Application.config.displayAttributes.width, Application.config.displayAttributes.height))

  gl.glMatrixMode(gl.GL_MODELVIEW)
  gl.glLoadIdentity()

  gl.glLineWidth(1.0)
  for x = 0, 2 do
    for y = 0, 2 do
      lost.gl.setColor(lost.common.Color(1,1,1))
      lost.gl.drawRectOutline(lost.math.Rect(x*Application.config.FieldSize.x, y*Application.config.FieldSize.y, Application.config.FieldSize.x, Application.config.FieldSize.y))
      if game.board[x][y].player == 0 then
        lost.gl.setColor(lost.common.Color(1,0,0))
        lost.gl.drawRectFilled(lost.math.Rect(x*Application.config.FieldSize.x+1, y*Application.config.FieldSize.y+1, Application.config.FieldSize.x-1, Application.config.FieldSize.y-1))
      elseif game.board[x][y].player == 1 then
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

function resizeHandler(event)
  local resizeEvent = lost.application.ResizeEvent.cast(event)

  lost.application.Application.config.FieldSize = lost.math.Vec2((resizeEvent.width / 3), (resizeEvent.height / 3))
  gl.glViewport(0, 0, resizeEvent.width, resizeEvent.height) gl.GLDEBUG()
end
