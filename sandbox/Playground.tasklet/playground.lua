require("lost.animation")

local Color = lost.common.Color

local view = nil

local logLines = {}
local wrap = function(f)
  local fenv = {}
  for k,v in next,_G do
    fenv[k] = v
  end
  fenv.log.debug = function(msg)
    table.insert(logLines, msg)
    view("log")("text"):text(table.concat(logLines, "\n"))
  end
  setfenv(f, fenv)
  return f
end

local compileCode = function(textview)
  local success, result = pcall(wrap(function()
    return assert(loadstring(textview:text()))
  end))

  if success then
    textview("success"):hidden(false)
    textview("error"):hidden(true)
  else
    textview("success"):hidden(true)
    textview("error"):hidden(false)
    textview("error")("text"):text(result)
  end
  return success, result
end

local runCode = function(textview)
  local success, result = compileCode(textview)
  if success then
    success, result = pcall(wrap(function()
      return result()
    end))
  end
  return success, result
end

local compile = function()
  compileCode(view("views")("class"))
  compileCode(view("views")("style"))
  compileCode(view("views")("testcode"))
end

local run = function()
  view("views")("result"):removeAllSubviews()
  local success = runCode(view("views")("class"))
  if success then
    success = runCode(view("views")("style"))
    if success then
      local success, result = runCode(view("views")("testcode"))
      if success then
        if result ~= nil and result.isDerivedFrom and result:isDerivedFrom("lost.guiro.view.View") then
          view("views")("result"):addSubview(result)
        end
      end
    end
  end
end

local successImage = lost.bitmap.Bitmap.create(tasklet.loader:load("images/success.png"))
successImage:premultiplyAlpha()

local errorImage = lost.bitmap.Bitmap.create(tasklet.loader:load("images/error.png"))
errorImage:premultiplyAlpha()

local Tween = lost.animation.Tween
local Bounds = lost.guiro.Bounds
local Rect = lost.math.Rect

local logAnimation = function(target, targetBoundsDef, method, duration, callback)
  local targetBounds = Bounds(unpack(targetBoundsDef))
  local targetRect = targetBounds:rect(target:superRect())
  targetRect = {x = targetRect.x, y = targetRect.y, w = targetRect.width, h = targetRect.height}
  local sourceRect = {x = target.rect.x, y = target.rect.y, w = target.rect.width, h = target.rect.height}

  return Tween(sourceRect, targetRect, {
    method = method or Tween.expoOut,
    duration = duration or 1,
    afterProcess = function(t)
      target:bounds(Bounds(sourceRect.x, sourceRect.y, sourceRect.w, sourceRect.h))
    end,
    onComplete = function(t)
      target:bounds(targetBounds)
      if type(callback) == "function" then
        callback()
      end
    end})
end

local _logAnimation = nil
local _logHidden = true
local toggleLog = function()
  if _logHidden then
    _logHidden = false
    if _logAnimation ~= nil then
      _logAnimation:stop()
    end
    view("log"):hidden(false)
    _logAnimation = logAnimation(view("log"), {"left",{"top", -30},"1",{".5", -15}}, nil, nil, function()
      _logAnimation = nil
    end)
  else
    _logHidden = true
    if _logAnimation ~= nil then
      _logAnimation:stop()
    end
    _logAnimation = logAnimation(view("log"), {"left",{"top", -30},"1",0}, nil, nil, function()
      _logAnimation = nil
      view("log"):hidden(true)
    end)
  end
end

tasklet.eventDispatcher:addEventListener(lost.application.KeyEvent.KEY_DOWN, function(event)
  if event.key == lost.application.K_C and event.altDown then
    compile()
  elseif event.key == lost.application.K_R and event.altDown then
    run()
  elseif event.key == lost.application.K_T and event.altDown then
    toggleLog()
  end
end)

view = lost.guiro.view.View
{
  id = "viewport",
  subviews =
  {
    lost.guiro.view.View
    {
      id = "controls",
      bounds = {"left", "top", "1", 30},
      subviews =
      {
        lost.guiro.view.Button
        {
          id = "compile",
          title = "Compile [Alt-c]",
          bounds = {{"left", 10}, "center", 100, 25},
          listeners =
          {
            buttonClick = function(event)
              compile()
            end
          }
        },
        lost.guiro.view.Button
        {
          id = "run",
          title = "Run [Alt-r]",
          bounds = {{"left", 120}, "center", 100, 25},
          listeners =
          {
            buttonClick = function(event)
              run()
            end
          }
        },
        lost.guiro.view.Button
        {
          id = "toggleLog",
          title = "Toggle Log [Alt-t]",
          bounds = {{"left", 230}, "center", 100, 25},
          listeners =
          {
            buttonClick = function(event)
              toggleLog()
            end
          }
        }
      }
    },
    lost.guiro.view.View
    {
      id = "views",
      bounds = {"left", {"top", -30}, "1", {"1", -30}},
      subviews =
      {
        lost.guiro.view.TextInput
        {
          id = "class",
          bounds = {"left", "top", ".5", ".5"},
          font = {"Vera mono", 10},
          multiLine = true,
          halign = "left",
          valign = "top",
          text =
[[
module("lost.guiro.view", package.seeall)

lost.common.Class "lost.guiro.view.TestLabel" "lost.guiro.view.Label" {}

function TestLabel:constructor(t)
  lost.guiro.view.Label.constructor(self, t)
  self:text("")
  self:addEventListener("mouseDown", function(event) self.textLayer:text("...except when it's clicked") end)
end

function TestLabel:text(...)
  if arg.n >= 1 then 
    self.textLayer:text("this text will never change...")
  else
    if self.textLayer then
      return self.textLayer:text()
    else
      return nil
    end
  end
end
]],
          listeners =
          {
            focusReceived = function(event)
              event.target:superview()("class_info"):hidden(true)
              event.target("success"):hidden(true)
              event.target("error"):hidden(true)
            end,
            focusLost = function(event)
              event.target:superview()("class_info"):hidden(false)
            end
          },
          subviews =
          {
            lost.guiro.view.Image
            {
              id = "success",
              bounds = {"center", "center", 16, 16},
              bitmap = successImage,
              flip = true,
              hidden = true
            },
            lost.guiro.view.View
            {
              id = "error",
              bounds = {"left", "top", "1", "1"},
              hidden = true,
              listeners =
              {
                mouseDown = function(event)
                  event.target:hidden(true)
                end
              },
              subviews =
              {
                lost.guiro.view.Image
                {
                  bounds = {"center", "center", 16, 16},
                  bitmap = errorImage,
                  flip = true
                },
                lost.guiro.view.Label
                {
                  id = "text",
                  bounds = {"center", {"center", -29}, "1", 26},
                  color = Color(1,0,0),
                  font = {"Vera mono", 12},
                  halign = "center",
                  breakMode = "word"
                }
              },
              sublayers =
              {
                lost.guiro.layer.Rect
                {
                  filled = true,
                  color = Color(.5, .5, .5, .8)
                }
              }
            }
          }
        },
        lost.guiro.view.View
        {
          id = "class_info",
          bounds = {{".5", -46}, {"top", -1}, 45, 25},
          subviews =
          {
            lost.guiro.view.Label
            {
              bounds = {5, 0, "1", "1"},
              color = Color(.3, .3, .3),
              font = {"Vera mono", 12},
              halign = "left",
              text = "Class"
            }
          },
          sublayers =
          {
            lost.guiro.layer.Rect
            {
              filled = true,
              color = Color(.8, .8, .8)
            }
          }
        },
        lost.guiro.view.TextInput
        {
          id = "style",
          bounds = {"right", "top", ".5", ".5"},
          font = {"Vera mono", 10},
          multiLine = true,
          halign = "center",
          valign = "top",
          text =
[[
local theme = lost.guiro.themeManager():getDefaultTheme()
theme:addStyle("lost.guiro.view.TestLabel", "default", function(target, args)
  target.layer:addSublayer(lost.guiro.layer.Rect{filled=true,color=lost.common.Color(1,0,0)})
  theme:labelDefault(target, args)
end)
]],
          listeners =
          {
            focusReceived = function(event)
              event.target:superview()("style_info"):hidden(true)
              event.target("success"):hidden(true)
              event.target("error"):hidden(true)
            end,
            focusLost = function(event)
              event.target:superview()("style_info"):hidden(false)
            end
          },
          subviews =
          {
            lost.guiro.view.Image
            {
              id = "success",
              bounds = {"center", "center", 16, 16},
              bitmap = successImage,
              flip = true,
              hidden = true
            },
            lost.guiro.view.View
            {
              id = "error",
              bounds = {"left", "top", "1", "1"},
              hidden = true,
              listeners =
              {
                mouseDown = function(event)
                  event.target:hidden(true)
                end
              },
              subviews =
              {
                lost.guiro.view.Image
                {
                  bounds = {"center", "center", 16, 16},
                  bitmap = errorImage,
                  flip = true
                },
                lost.guiro.view.Label
                {
                  id = "text",
                  bounds = {"center", {"center", -29}, "1", 26},
                  color = Color(1,0,0),
                  font = {"Vera mono", 12},
                  halign = "center",
                  breakMode = "word"
                }
              },
              sublayers =
              {
                lost.guiro.layer.Rect
                {
                  filled = true,
                  color = Color(.5, .5, .5, .8)
                }
              }
            }
          }
        },
        lost.guiro.view.View
        {
          id = "style_info",
          bounds = {{"right", -1}, {"top", -1}, 45, 25},
          subviews =
          {
            lost.guiro.view.Label
            {
              bounds = {5, 0, "1", "1"},
              color = Color(.3, .3, .3),
              font = {"Vera mono", 12},
              halign = "left",
              text = "Style"
            }
          },
          sublayers =
          {
            lost.guiro.layer.Rect
            {
              filled = true,
              color = Color(.8, .8, .8)
            }
          }
        },
        lost.guiro.view.TextInput
        {
          id = "testcode",
          bounds = {"left", "bottom", ".5", ".5"},
          font = {"Vera mono", 10},
          multiLine = true,
          halign = "right",
          valign = "top",
          text = 
[[
return lost.guiro.view.TestLabel
{
  bounds = {0, 0, "1", "1"}
}

!!! REMOVE THIS LINE TO SEE A RESULT !!!
]],
          listeners =
          {
            focusReceived = function(event)
              event.target:superview()("testcode_info"):hidden(true)
              event.target("success"):hidden(true)
              event.target("error"):hidden(true)
            end,
            focusLost = function(event)
              event.target:superview()("testcode_info"):hidden(false)
            end
          },
          subviews =
          {
            lost.guiro.view.Image
            {
              id = "success",
              bounds = {"center", "center", 16, 16},
              bitmap = successImage,
              flip = true,
              hidden = true
            },
            lost.guiro.view.View
            {
              id = "error",
              bounds = {"left", "top", "1", "1"},
              hidden = true,
              listeners =
              {
                mouseDown = function(event)
                  event.target:hidden(true)
                end
              },
              subviews =
              {
                lost.guiro.view.Image
                {
                  bounds = {"center", "center", 16, 16},
                  bitmap = errorImage,
                  flip = true
                },
                lost.guiro.view.Label
                {
                  id = "text",
                  bounds = {"center", {"center", -21}, "1", 26},
                  color = Color(1,0,0),
                  font = {"Vera mono", 12},
                  halign = "center",
                  breakMode = "word"
                }
              },
              sublayers =
              {
                lost.guiro.layer.Rect
                {
                  filled = true,
                  color = Color(.5, .5, .5, .8)
                }
              }
            }
          }
        },
        lost.guiro.view.View
        {
          id = "testcode_info",
          bounds = {{"center", -34}, {"center", -13}, 66, 25},
          subviews =
          {
            lost.guiro.view.Label
            {
              bounds = {5, 0, "1", "1"},
              color = Color(.3, .3, .3),
              font = {"Vera mono", 12},
              halign = "left",
              text = "Testcode"
            }
          },
          sublayers =
          {
            lost.guiro.layer.Rect
            {
              filled = true,
              color = Color(.8, .8, .8)
            }
          }
        },
        lost.guiro.view.View
        {
          id = "result",
          bounds = {"right", "bottom", ".5", ".5"},
          listeners =
          {
            focusReceived = function(event)
              event.target:superview()("result_info"):hidden(true)
            end,
            focusLost = function(event)
              event.target:superview()("result_info"):hidden(false)
            end
          },
          sublayers =
          {
            lost.guiro.layer.Rect
            {
              filled = false,
              color = Color(0,0,0),
              width = 1
            }
          }
        },
        lost.guiro.view.View
        {
          id = "result_info",
          bounds = {{"right", -1}, {"center", -13}, 52, 25},
          subviews =
          {
            lost.guiro.view.Label
            {
              bounds = {5, 0, "1", "1"},
              color = Color(.3, .3, .3),
              font = {"Vera mono", 12},
              halign = "left",
              text = "Result"
            }
          },
          sublayers =
          {
            lost.guiro.layer.Rect
            {
              filled = true,
              color = Color(.8, .8, .8)
            }
          }
        }
      }
    },
    lost.guiro.view.View
    {
      id = "log",
      bounds = {"left",{"top", -30},"1",0},
      hidden = true,
      subviews =
      {
        lost.guiro.view.Label
        {
          id = "text",
          clip = true,
          color = Color(1, 0, 0),
          font = {"Vera mono", 12},
          halign = "left",
          valign = "top"
        }
      },
      sublayers =
      {
        lost.guiro.layer.Rect
        {
          filled = true,
          color = Color(.8, .8, .8, .75)
        },
        lost.guiro.layer.Rect
        {
          filled = false,
          color = Color(0,0,0),
          width = 2
        }
      }
    }
  }
}

-- enable focus events
view("views")("result").focusable = true

return view

