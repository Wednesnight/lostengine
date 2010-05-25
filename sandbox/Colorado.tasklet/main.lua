require("lost.declarative.Context")
require("lost.common.Shaders")

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.application.WindowParams"
using "lost.common.Color"

windowParams = WindowParams("Colorado", Rect(50,50,640,480))

local running = true
local dcl = nil
local _tasklet = nil

local boardPos = Vec2(75,75)
local boardSize = Vec2(4,4)
local boardColors = {
  Color(.75,0,0),
  Color(.75,.75,0),
  Color(0,.75,0),
  Color(0,.75,.75),
  Color(0,0,.75),
  Color(.25,0,0),
  Color(0,.25,0),
  Color(0,0,.25),
  Color(.25,.25,0),
  Color(0,.25,.25),
  Color(.25,0,.25)
}
local activeColors = nil
local numStartBlocksPerColor = 2
local maxColors = nil
local blockSize = Vec2((windowParams.rect.width - boardPos.x*2) / (boardSize.x + 2), (windowParams.rect.height - boardPos.y*2) / (boardSize.y + 2))

local board = nil
local boardNodes = nil
local boardNode = nil
local currentBlock = nil
local startBlocks = nil

local screen = nil
local score = nil
local scoreLabel = nil

local finished = false
local finishNode = nil

local sounds = nil

function startup(tasklet)
  _tasklet = tasklet

  tasklet.name = "Colorado"
  tasklet.waitForEvents = true

  tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, keyHandler)  
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, mouseClickHandler)  

  sounds = require("sounds")

  dcl = lost.declarative.Context(tasklet.loader)
  
  tasklet.renderNode:add(
    dcl.rg:Node
    {
      dcl.rg:ClearColor
      {
        color = Color(0,0,0,1)
      },
      dcl.rg:Clear
      {
        mask = gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT
      },
      dcl.rg:Camera2D
      {
        viewport = Rect(0, 0, windowParams.rect.width, windowParams.rect.height)
      },    
      dcl.rg:DepthTest{ false }
    }
  )

  local boardFrame = lost.mesh.Rect.create(Rect(boardPos.x, boardPos.y, windowParams.rect.width - boardPos.x*2, windowParams.rect.height - boardPos.y*2))
  boardFrame.material.shader = lost.common.Shaders.colorShader()
  boardFrame.material.color = Color(1,1,1)
  tasklet.renderNode:add(
    dcl.rg:Draw
    {
      mesh = boardFrame
    }
  )

  -- UI
  scoreLabel = dcl.guiro:Label
  {
    bounds = {50, "top", 50, 25},
    text = "0",
    showFrame = true,
    frameWidth = 2,
    showBackground = true,
    backgroundColor = Color(.25,.25,.25),
    backgroundCornerRadius = 20,
    frameCornerRadius = 20,
    backgroundRoundCorners = {false, true, false, false},
    frameRoundCorners = {false, true, false, false},
    frameShowSides = {true, true, false, true}
  }
  finishNode = dcl.guiro:Label
  {
    bounds = {"center", "center", 300, 50},
    fontSize = 16,
    text = "Game Over (Press Space to restart)",
    showFrame = true,
    frameWidth = 2,
    showBackground = true,
    backgroundColor = Color(.25,.25,.25,.75),
    backgroundCornerRadius = 20,
    frameCornerRadius = 20,
    backgroundRoundCorners = {true, true, true, true},
    frameRoundCorners = {true, true, true, true}
  }
  screen = dcl.guiro:Screen
  {
    dcl.guiro:UserInterface
    {
      bounds = {"left", "bottom", "1", "1"},
      dcl.guiro:Window
      {
        bounds = {"left", "bottom", "1", "1"},
        showBackground = false,
        dcl.guiro:Label
        {
          bounds = {"left", "top", 50, 25},
          showFrame = true,
          frameWidth = 2,
          showBackground = true,
          backgroundColor = Color(.25,.25,.25),
          frameShowSides = {true, true, true, false},
          text = "Score:"
        },
        scoreLabel,
        finishNode
      }
    }
  }

  -- setup
  setup()

  return true
end

function setup()
  finished = false
  finishNode:hidden(true)

  if boardNode ~= nil then
    _tasklet.renderNode:remove(boardNode)
  end

  board = {}
  boardNodes = {}
  boardNode = lost.rg.Node.create()
  maxColors = math.min(math.floor((boardSize.x + boardSize.y) / numStartBlocksPerColor), #boardColors)
  activeColors = {}
  for idx = 1,maxColors do
    table.insert(activeColors, boardColors[idx])
  end
  score = 0
  scoreLabel:text(tostring(score))

  for x = 2,boardSize.x+1 do
    if board[x] == nil then
      board[x] = {}
    end
    if boardNodes[x] == nil then
      boardNodes[x] = {}
    end
    for y = 2,boardSize.y+1 do

      board[x][y] = dcl.mesh:Quad
      {
        rect = Rect(boardPos.x + blockSize.x * (x-1), boardPos.y + blockSize.y * (y-1), blockSize.x, blockSize.y),
        material = 
        {
          blend = true,
          color = randomColor()
        }
      }

      boardNodes[x][y] = dcl.rg:Draw { mesh = board[x][y] }
      boardNode:add(boardNodes[x][y])

    end
  end

  startBlocks = {}
  local currentPos = 1
  -- top
  for pos = 1, boardSize.x+2 do
    table.insert(startBlocks, currentPos, Vec2(pos, boardSize.y+2))
    currentPos = currentPos + 1
    pos = pos + 1
  end
  -- right
  for pos = 1, boardSize.y+1 do
    table.insert(startBlocks, currentPos, Vec2(boardSize.x+2, (boardSize.y+2) - pos))
    currentPos = currentPos + 1
    pos = pos + 1
  end
  -- bottom
  for pos = 1, boardSize.x+1 do
    table.insert(startBlocks, currentPos, Vec2((boardSize.x+2) - pos, 1))
    currentPos = currentPos + 1
    pos = pos + 1
  end
  -- left
  for pos = 2, boardSize.y+1 do
    table.insert(startBlocks, currentPos, Vec2(1, pos))
    currentPos = currentPos + 1
    pos = pos + 1
  end
  local idx = 2
  local colorIdx = 1
  local colorNum = 1
  local validSlots = {}
  while idx <= boardSize.x*2 + boardSize.y*2 + 4 do
    table.insert(validSlots, idx)
    idx = idx + 2
    if idx == boardSize.x+2 or idx == boardSize.x+3 or idx == boardSize.x+boardSize.y+3 or idx == boardSize.x+boardSize.y+4 or
       idx == boardSize.x*2+boardSize.y+4 or idx == boardSize.x*2+boardSize.y+5
    then
      idx = idx + 1
    end
  end
  while colorNum <= maxColors do
    local slot = math.random(#validSlots)
    local coord = startBlocks[validSlots[slot]]
    table.remove(validSlots, slot)
    local bounds = Rect(boardPos.x + blockSize.x * (coord.x-1), boardPos.y + blockSize.y * (coord.y-1), blockSize.x, blockSize.y)

    if board[coord.x] == nil then
      board[coord.x] = {}
    end

    board[coord.x][coord.y] = dcl.mesh:Quad
    {
      rect = bounds,
      material = 
      {
        blend = true,
        color = activeColors[colorNum]
      }
    }

    if boardNodes[coord.x] == nil then
      boardNodes[coord.x] = {}
    end
    boardNodes[coord.x][coord.y] = dcl.rg:Draw { mesh = board[coord.x][coord.y] }
    boardNode:add(boardNodes[coord.x][coord.y])

    local frame = lost.mesh.Rect.create(bounds)
    frame.material.shader = lost.common.Shaders.colorShader()
    frame.material.color = Color(1,1,1)
    boardNode:add(
      dcl.rg:Draw
      {
        mesh = frame
      }
    )
  
    colorIdx = colorIdx + 1
    colorNum = math.ceil(colorIdx / numStartBlocksPerColor)
  end

  -- clear complete paths
  for x = 2,boardSize.x+1 do
    for y = 2,boardSize.y+1 do
      if board[x][y] ~= nil then
        local color = board[x][y].material.color
        local stack = {[x] = {[y] = true}}
        local path = checkPath(color, x, y, stack)
        if path ~= nil then
          local path2 = checkPath(color, x, y, stack)
          if path2 ~= nil then
            table.insert(path, 1, Vec2(x,y))
            for idx = 1,#path2 do
              table.insert(path,1,path2[idx])
            end
            score = score + clearPath(path)
            clearStartBlocksAndColors()
            sounds.pathRemoved:play()
          end
        end

        scoreLabel:text(tostring(score))
        if #activeColors == 0 then
          finish()
        end
      end
    end
  end

  local currentBounds = Rect(windowParams.rect.width / 2 - blockSize.x / 2,
                             windowParams.rect.height - boardPos.y + (boardPos.y - blockSize.y) / 2,
                             blockSize.x, blockSize.y)
  currentBlock = dcl.mesh:Quad
  {
    rect = currentBounds,
    material = 
    {
      blend = true,
      color = randomColor()
    }
  }

  boardNode:add(
    dcl.rg:Draw
    {
      mesh = currentBlock
    }
  )

  local currentFrame = lost.mesh.Rect.create(currentBounds)
  currentFrame.material.shader = lost.common.Shaders.colorShader()
  currentFrame.material.color = Color(1,1,1)
  boardNode:add(
    dcl.rg:Draw
    {
      mesh = currentFrame
    }
  )

  _tasklet.renderNode:add(boardNode)

end

function update(tasklet)
  return running
end

function keyHandler(event)
  if (event.key == lost.application.K_ESCAPE) then
    running = false
  elseif (event.key == lost.application.K_SPACE) then
    setup()
  end
end

function randomColor()
  return activeColors[math.random(maxColors)]
end

function checkColor(color, target)
  local result = 0
  if target ~= nil and target.material.color == color then
    result = 1
  end
  return result
end

function checkPath(color, x, y, stack, complete)
  local result = nil
  local dir = {left = Vec2(x-1,y), right = Vec2(x+1,y), up = Vec2(x,y+1), down = Vec2(x,y-1)}
  for k,v in next,dir do
    if board[v.x] ~= nil and board[v.x][v.y] ~= nil and (stack[v.x] == nil or stack[v.x][v.y] ~= true) and
       (complete or board[v.x][v.y].material.color == color)
    then
--      if complete then log.debug("checking ".. tostring(v)) end
      if stack[v.x] == nil then
        stack[v.x] = {}
      end
      stack[v.x][v.y] = true
      if (v.x == 1 or v.x == boardSize.x+2 or v.y == 1 or v.y == boardSize.y+2) and
        (not complete or board[v.x][v.y].material.color == color)
      then
--        if complete then log.debug(tostring(v)) end
        result = {v}
        break
      end
      local nextResult = checkPath(color, v.x, v.y, stack,complete)
      if nextResult ~= nil then
--        if complete then log.debug(tostring(v)) end
        result = {v}
        for nk,nv in next,nextResult do
          table.insert(result, nv)
        end
        break
      end
    end
  end
  return result
end

function clearPath(path)
  local result = 0
  local lastPos = nil
  for k,v in next,path do
    if lastPos ~= nil then
      boardNode:add(dcl.rg:Draw {
        mesh = dcl.mesh:Line {
          start = Vec2(boardPos.x + lastPos.x * blockSize.x - blockSize.x / 2, boardPos.y + lastPos.y * blockSize.y - blockSize.y / 2),
          finish = Vec2(boardPos.x + v.x * blockSize.x - blockSize.x / 2, boardPos.y + v.y * blockSize.y - blockSize.y / 2)
        }
      })
    end
    lastPos = v
    if boardNodes[v.x] ~= nil and boardNodes[v.x][v.y] ~= nil then
      if result == 0 then
        result = 1
      else
        result = result * 2
      end
      boardNode:remove(boardNodes[v.x][v.y])
      boardNodes[v.x][v.y] = nil
      board[v.x][v.y] = nil
    end
  end
  return result
end

function clearStartBlocksAndColors()
  local remaining = {}
  for k,v in next,startBlocks do
    if board[v.x] ~= nil and board[v.x][v.y] ~= nil then
      local stack = {[v.x] = {[v.y] = true}}
--      log.debug("CHECK")
      local path = checkPath(board[v.x][v.y].material.color,v.x,v.y,stack,true)
      if path == nil then
--        log.debug("REMOVE")
        boardNode:remove(boardNodes[v.x][v.y])
        boardNodes[v.x][v.y] = nil
        board[v.x][v.y] = nil
      else
--        log.debug("OK")
        table.insert(remaining, board[v.x][v.y].material.color)
      end
    end
  end
  for idx = #activeColors,1,-1 do
    local color = activeColors[idx]
    local found = 0
    for k,v in next,remaining do
      if v == color then
        found = found + 1
      end
    end
    if found <= 1 then
      for k,v in next,startBlocks do
        if board[v.x] ~= nil and board[v.x][v.y] ~= nil and board[v.x][v.y].material.color == color then
          boardNode:remove(boardNodes[v.x][v.y])
          boardNodes[v.x][v.y] = nil
          board[v.x][v.y] = nil
        end
      end
      table.remove(activeColors,idx)
      maxColors = maxColors - 1
    end
  end
end

function finish()
  finished = true
  finishNode:hidden(false)
  sounds.levelFinished:play()
end

function mouseClickHandler(event)
  if not finished then
    local x, y = math.ceil((event.pos.x - boardPos.x) / blockSize.x), math.ceil((event.pos.y - boardPos.y) / blockSize.y)
    if x > 1 and x < boardSize.x + 2 and y > 1 and y < boardSize.y + 2 and board[x][y] ~= nil then
      local color = currentBlock.material.color
      if board[x][y].material.color ~= color then

        local block = board[x][y]
        block.material.color = color
        sounds.blockRemoved:play()

        local points = checkColor(color, board[x-1][y]) + checkColor(color, board[x+1][y]) +
                       checkColor(color, board[x][y-1]) + checkColor(color, board[x][y+1])
        score = score + points

        local stack = {[x] = {[y] = true}}
        local path = checkPath(color, x, y, stack)
        if path ~= nil then
          local path2 = checkPath(color, x, y, stack)
          if path2 ~= nil then
            table.insert(path, 1, Vec2(x,y))
            for idx = 1,#path2 do
              table.insert(path,1,path2[idx])
            end
            score = score + clearPath(path)
            clearStartBlocksAndColors()
            sounds.pathRemoved:play()
          end
        end

        scoreLabel:text(tostring(score))
        if #activeColors == 0 then
          finish()
        else
          currentBlock.material.color = randomColor()
        end
      end
    end
  end
end
