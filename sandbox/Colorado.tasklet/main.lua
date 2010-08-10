require("lost.declarative.Context")
require("lost.common.Shaders")

require("animations.AnimationManager")
require("animations.BlockScoreAnimation")

using "lost.math.Vec2"
using "lost.math.Rect"
using "lost.common.Color"
using "lost.math.MatrixTranslation"
using "lost.math.Vec3"

local running = true
local dcl = nil
local _tasklet = nil

local boardPos = Vec2(100, 100)
local boardSize = Vec2(8,8)
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
local numStartBlocksPerColor = 4
local maxColors = nil
local config = require("config")
local blockSize = Vec2((config.window.width - boardPos.x*2) / (boardSize.x + 2),
                       (config.window.height - boardPos.y*2) / (boardSize.y + 2))

local board = nil
local boardNodes = nil
local startFrames = nil
local boardNode = nil
local currentBlock = nil
local currentFrame = nil
local startBlocks = nil
local boardFrame = nil

local screen = nil
local score = nil
local scoreLabel = nil
local scores = require("resources.scores")
local scoresChanged = false

local initialized = false
local finished = true
local scoreboard = nil
local scoretable = nil
local endScore = nil
local buttons = nil

local sounds = nil

local scorePrefix = " Score: "

local animationManager = nil

-- Initialize the pseudo random number generator
math.randomseed(os.time())

function startup()
  _tasklet = tasklet
  tasklet.eventDispatcher:addEventListener(lost.application.MouseEvent.MOUSE_DOWN, mouseClickHandler)
  tasklet.eventDispatcher:addEventListener(lost.application.ResizeEvent.MAIN_WINDOW_RESIZE, resizeHandler)

  sounds = require("resources/sounds")

  dcl = lost.declarative.Context(tasklet.loader)

  camera = dcl.rg:Camera2D {
    viewport = Rect(0, 0, config.window.width, config.window.height)
  }
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
      camera,    
      dcl.rg:DepthTest{ false }
    }
  )

  boardFrame = lost.mesh.Rect.create(Rect(boardPos.x, boardPos.y, config.window.width - boardPos.x*2, config.window.height - boardPos.y*2))
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
    bounds = {"left", "top", 100, 25},
    text = scorePrefix.."0",
    halign = "left",
    showFrame = true,
    frameWidth = 2,
    showBackground = true,
    backgroundColor = Color(.25,.25,.25),
    backgroundCornerRadius = 10,
    frameCornerRadius = 10,
    backgroundRoundCorners = {false, true, false, false},
    frameRoundCorners = {false, true, false, false}
  }

  soundLabel = dcl.guiro:Label
  {
    bounds = {"right", "top", 75, 25},
    text = "Sound: On",
    showFrame = true,
    frameWidth = 2,
    showBackground = true,
    backgroundColor = Color(.25,.25,.25),
    backgroundCornerRadius = 10,
    frameCornerRadius = 10,
    backgroundRoundCorners = {true, false, false, false},
    frameRoundCorners = {true, false, false, false},
    listeners = {
      mouseDown = function(event)
        if event.target == event.currentTarget then
          toggleSound()
        end
      end
    }
  }

  endScore = dcl.guiro:Label
  {
    bounds = {"center", "bottom", 150, 50},
    fontSize = 16,
    text = "",
    textColor = Color(0,1,0),
    showFrame = true,
    frameWidth = 1,
    showBackground = false,
    frameShowSides = {true, false, false, false}
  }
  scoreboard = dcl.guiro:Window
  {
    bounds = {"left", "top", "1", {"1", -50}},
    focusable = false,
    backgroundColor = Color(0,0,0),
    showFrame = false,
    dcl.guiro:Label
    {
      bounds = {"center", "top", 150, 50},
      fontSize = 16,
      text = "Highscores",
      showFrame = true,
      frameWidth = 1,
      showBackground = false,
      frameShowSides = {false, true, false, false}
    },
    endScore
  }
  rebuildScoretable()

  buttons = dcl.guiro:Window {
    bounds = {"left", "bottom", "1", 50},
    showBackground = false,
    showFrame = false,
    dcl.guiro:Button
    {
      id = "newGame",
      bounds = {{"right", -120}, {"bottom", 10}, 100, 30},
      title = "New Game",
      listeners = {
        buttonClick = function(event)
          if event.target == event.currentTarget then
            setup()
          end
        end
      }
    },
    dcl.guiro:Button
    {
      id = "restart",
      hidden = true,
      bounds = {{"right", -120}, {"bottom", 10}, 100, 30},
      title = "Restart",
      listeners = {
        buttonClick = function(event)
          if event.target == event.currentTarget then
            setup()
          end
        end
      }
    },
    dcl.guiro:Button
    {
      id = "quit",
      bounds = {{"right", -10}, {"bottom", 10}, 100, 30},
      title = "Quit",
      listeners = {
        buttonClick = function(event)
          if event.target == event.currentTarget then
            running = false
          end
        end
      }
    }
  }

  screen = dcl.guiro:Screen
  {
    dcl.guiro:UserInterface
    {
      bounds = {"left", "bottom", "1", "1"},
      dcl.guiro:Window
      {
        bounds = {"left", "top", "1", 50},
        focusable = false,
        showBackground = false,
        showFrame = false,
        scoreLabel,
        soundLabel
      },
      scoreboard,
      buttons
    }
  }

  animationManager = AnimationManager(tasklet.loader, boardNode, board)

  return true
end

function shutdown()
  if scoresChanged then
    local scoresFile, err = io.open(tasklet.loader:locate("resources/scores.lua"), "w+")
    if scoresFile ~= nil then
      scoresFile:write("return {\n")
      for k,v in next,scores do
        scoresFile:write("  {\n")
        scoresFile:write("    points = ".. tostring(v.points) ..",\n")
        scoresFile:write("    date = \"".. v.date .."\"\n")
        if k < #scores then
          scoresFile:write("  },\n")
        else
          scoresFile:write("  }\n")
        end
      end
      scoresFile:write("}\n")
      scoresFile:flush()
      scoresFile:close()
    else
      log.error(err)
    end
  end
end

function setup()
  initialized = true
  finished = false
  scoreboard:hidden(true)
  buttons("newGame"):hidden(true)
  buttons("restart"):hidden(false)

  if boardNode ~= nil then
    _tasklet.renderNode:remove(boardNode)
  end

  board = {}
  boardNodes = {}
  boardNode = lost.rg.Node.create()
  animationManager.renderNode = boardNode
  animationManager.board = board
  maxColors = math.min(math.floor((boardSize.x + boardSize.y) / numStartBlocksPerColor), #boardColors)
  activeColors = {}
  for idx = 1,maxColors do
    table.insert(activeColors, boardColors[idx])
  end
  score = 0
  scoreLabel:text(scorePrefix..tostring(score))

  for x = 2,boardSize.x+1 do
    if board[x] == nil then
      board[x] = {}
    end
    if boardNodes[x] == nil then
      boardNodes[x] = {}
    end
    for y = 2,boardSize.y+1 do

      local bounds = Rect(boardPos.x + blockSize.x * (x-1), boardPos.y + blockSize.y * (y-1), blockSize.x, blockSize.y)

      board[x][y] = dcl.mesh:Quad
      {
        rect = bounds,
        material = 
        {
          blend = true,
          color = randomColor()
        }
      }

      boardNodes[x][y] = dcl.rg:Draw { mesh = board[x][y] }
      boardNode:add(boardNodes[x][y])

      local frame = lost.mesh.Rect.create(bounds)
      frame.material.shader = lost.common.Shaders.colorShader()
      frame.material.color = lost.common.Color(1,1,1)
      boardNode:add(
        dcl.rg:Draw
        {
          mesh = frame
        }
      )

    end
  end

  startBlocks = {}
  startFrames = {}
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

    if startFrames[coord.x] == nil then
      startFrames[coord.x] = {}
    end
    local frame = lost.mesh.Rect.create(bounds)
    frame.material.shader = lost.common.Shaders.colorShader()
    frame.material.color = lost.common.Color(1,1,1)
    boardNode:add(
      dcl.rg:Draw
      {
        mesh = frame
      }
    )
    startFrames[coord.x][coord.y] = frame
  
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
            sounds:play("pathRemoved")
          end
        end

        scoreLabel:text(scorePrefix..tostring(score))
        if #activeColors == 0 then
          finish()
        end
      end
    end
  end

  local currentBounds = Rect(config.window.width / 2 - blockSize.x / 2,
                             config.window.height - boardPos.y + (boardPos.y - blockSize.y) / 2,
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

  currentFrame = lost.mesh.Rect.create(currentBounds)
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

function rebuildScoretable(newIndex)
  if scoretable ~= nil then
    scoreboard:removeSubview(scoretable)
  end

  scoretable = dcl.guiro:VBox {
    bounds = {"left", {"top", -60}, "1", "1"},
    backgrondColor = Color(0,0,0),
    mode = "stack"
  }
  
  local fontSize, fontColor = 14, Color(1,0,0)
  for k,v in next,scores do
    if newIndex ~= nil and k == newIndex then
      fontSize = 14
      fontColor = Color(0,1,0)
    elseif k > 1 then
      fontSize = 12
      fontColor = Color(1,1,1)
    end
    scoretable:addSubview(dcl.guiro:HBox {
      bounds = {nil, nil, 150, 50},
      mode = "stack",
      dcl.guiro:Label
      {
        bounds = {nil, nil, 25, 50},
        fontSize = fontSize,
        textColor = fontColor,
        text = tostring(k) ..".",
        halign = "left",
        showBackground = false
      },
      dcl.guiro:Label
      {
        bounds = {nil, nil, 100, 50},
        fontSize = fontSize,
        textColor = fontColor,
        text = tostring(v.points),
        halign = "right",
        showBackground = false
      },
      dcl.guiro:Label
      {
        bounds = {nil, nil, 150, 50},
        fontSize = fontSize,
        textColor = fontColor,
        text = "[".. tostring(v.date) .."]",
        halign = "right",
        showBackground = false
      }
    })
  end

  scoreboard:addSubview(scoretable)
end

function update()
  if running then
    if not animationManager:process() then
      tasklet.waitForEvents = false
    end
  end
  return running
end

function toggleSound()
  sounds.enabled = not sounds.enabled
  if sounds.enabled then
    soundLabel:text("Sound: On")
  else
    soundLabel:text("Sound: Off")
  end
end

function resizeHandler(event)
  config.window.width = event.width
  config.window.height = event.height

  camera.cam:viewport(Rect(0,0,event.width,event.height))

  blockSize = Vec2((event.width - boardPos.x*2) / (boardSize.x + 2), (event.height - boardPos.y*2) / (boardSize.y + 2))

  if initialized then
    local currentBounds = Rect(event.width / 2 - blockSize.x / 2,
                               event.height - boardPos.y + (boardPos.y - blockSize.y) / 2,
                               blockSize.x, blockSize.y)

    currentBlock:updateSize(Vec2(currentBounds.width, currentBounds.height))
    currentBlock.transform = MatrixTranslation(Vec3(currentBounds.x, currentBounds.y, 0))
    currentFrame:updateSize(Vec2(currentBounds.width, currentBounds.height))
    currentFrame.transform = MatrixTranslation(Vec3(currentBounds.x, currentBounds.y, 0))

    boardFrame:updateSize(Vec2(event.width - boardPos.x*2, event.height - boardPos.y*2))
    for x,ys in next,board do
      for y,mesh in next,ys do
        board[x][y]:updateSize(Vec2(blockSize.x, blockSize.y))
        board[x][y].transform = MatrixTranslation(Vec3(boardPos.x + blockSize.x * (x-1), boardPos.y + blockSize.y * (y-1), 0))
      end
    end
    for x,ys in next,startFrames do
      for y,mesh in next,ys do
        startFrames[x][y]:updateSize(Vec2(blockSize.x, blockSize.y))
        startFrames[x][y].transform = MatrixTranslation(Vec3(boardPos.x + blockSize.x * (x-1), boardPos.y + blockSize.y * (y-1), 0))
      end
    end
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
  local blockScore = 10
  local factor = 1
  local lastPos = nil
  for k,v in next,path do
    if lastPos ~= nil then
      local color = lost.common.Color(1,1,1)
      if board[v.x] ~= nil and board[v.x][v.y] ~= nil then
        color = board[v.x][v.y].material.color
      end
      boardNode:add(dcl.rg:Draw {
        mesh = dcl.mesh:Line {
          start = Vec2(boardPos.x + lastPos.x * blockSize.x - blockSize.x / 2, boardPos.y + lastPos.y * blockSize.y - blockSize.y / 2),
          finish = Vec2(boardPos.x + v.x * blockSize.x - blockSize.x / 2, boardPos.y + v.y * blockSize.y - blockSize.y / 2),
          material = {
            color = color
          }
        }
      })
    end
    lastPos = v
    if boardNodes[v.x] ~= nil and boardNodes[v.x][v.y] ~= nil then

      animationManager:add(BlockScoreAnimation(blockSize, boardSize, boardPos, v, "x"..factor))
      result = result + blockScore*factor
      factor = factor + 1
      _tasklet.waitForEvents = false

      boardNode:remove(boardNodes[v.x][v.y])
      boardNodes[v.x][v.y] = nil
      if board[v.x] ~= nil and board[v.x][v.y] ~= nil and startFrames[v.x] ~= nil and startFrames[v.x][v.y] ~= nil then
        startFrames[v.x][v.y].material.color = board[v.x][v.y].material.color
      end
      board[v.x][v.y] = nil
    end
  end
  return result
end

function clearStartBlocksAndColors()
  local result = 0
  local remaining = {}
  for k,v in next,startBlocks do
    if board[v.x] ~= nil and board[v.x][v.y] ~= nil then
      local stack = {[v.x] = {[v.y] = true}}
--      log.debug("CHECK")
      local path = checkPath(board[v.x][v.y].material.color,v.x,v.y,stack,true)
      if path == nil then
--        log.debug("REMOVE")
        result = result - 10
        animationManager:add(BlockScoreAnimation(blockSize, boardSize, boardPos, v, result))
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
          result = result - 10
          animationManager:add(BlockScoreAnimation(blockSize, boardSize, boardPos, v, result))
          boardNode:remove(boardNodes[v.x][v.y])
          boardNodes[v.x][v.y] = nil
          board[v.x][v.y] = nil
        end
      end
      table.remove(activeColors,idx)
      maxColors = maxColors - 1
    end
  end
  return result
end

function finish()
  finished = true
  for k,v in next,scores do
    if score > v.points then
      scoresChanged = true
      local newEntry = {points = score, date = os.date("%Y-%m-%d")}
      table.insert(scores, k, newEntry) -- add new score
      table.remove(scores)              -- remove last
      rebuildScoretable(k)
      break
    end
  end
  endScore:text("Score: ".. tostring(score))
  scoreboard:hidden(false)
  buttons("newGame"):hidden(false)
  buttons("restart"):hidden(true)
  sounds:play("levelFinished")
end

function mouseClickHandler(event)
  if not finished then
    local x, y = math.ceil((event.pos.x - boardPos.x) / blockSize.x), math.ceil((event.pos.y - boardPos.y) / blockSize.y)
    if x > 1 and x < boardSize.x + 2 and y > 1 and y < boardSize.y + 2 and board[x][y] ~= nil then
      local color = currentBlock.material.color
      if board[x][y].material.color ~= color then

        local block = board[x][y]
        block.material.color = color
        sounds:play("blockRemoved")

--        local points = checkColor(color, board[x-1][y]) + checkColor(color, board[x+1][y]) +
--                       checkColor(color, board[x][y-1]) + checkColor(color, board[x][y+1])
--        score = score + points

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
            score = score + clearStartBlocksAndColors()
            sounds:play("pathRemoved")
          end
        end

        scoreLabel:text(scorePrefix..tostring(score))
        if #activeColors == 0 then
          finish()
        else
          currentBlock.material.color = randomColor()
        end
      end
    end
  end
end
