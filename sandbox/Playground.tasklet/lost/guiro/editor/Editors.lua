module("lost.guiro.editor", package.seeall)

require("lost.guiro")

local View = lost.guiro.view.View
local TextInput = lost.guiro.view.TextInput
local Label = lost.guiro.view.Label

Editors = {}

function Editors.get(v, c)
  if type(v) == "table" and type(v.className) == "function" then

    local result = Editors[v:className()]
    if result ~= nil then
      return result(v, c)
    end
    
    if type(v.bases) == "table" then
      for i = table.maxn(v.bases) - 1,0,-1 do
        result = Editors[v.bases[i]]
        if result ~= nil then
          return result(v, c)
        end
      end
    end
  end
  return nil
end

Editors["lost.guiro.Bounds"] = function(v, c)
  local apply = function(e)
    local text = e.target:text()
    if string.len(text) > 0 then
      local value = tonumber(text)
      if value == nil then
        local success = false
        success, value = pcall(function()
          return assert(loadstring("return ".. text))()
        end)
        if not success then
          log.error(value)
          value = nil
        end
      end

      if value ~= nil then
        v.values[tonumber(e.target.id)] = value
        v.x,v.y,v.width,v.height = lost.guiro.Bounds.decode(v.values)
        c(v)
      end
    end
  end

  local getValue = nil
  getValue = function(v)
    if type(v) == "number" then
      return tostring(v)
    elseif type(v) == "string" then
      return "\"".. v .."\""
    elseif type(v) == "table" then
      local result = ""
      for _k,_v in next,v do
        if string.len(result) == 0 then
          result = "{"
        else
          result = result ..", "
        end
        result = result .. getValue(_v)
      end
      return result .."}"
    end
  end

  return View {
    bounds = {"left", "top", "1", 60},
    subviews = {
      Label {
        bounds = {"left", "top", "1", ".3"},
        halign = "left",
        text = "Bounds:"
      },
      TextInput {
        id = "1",
        font = {"Vera mono", 10},
        bounds = {"left", {"bottom", ".35"}, ".5", ".35"},
        text = getValue(v.values[1]),
        listeners = {
          enterPressed = apply
        }
      },
      TextInput {
        id = "2",
        font = {"Vera mono", 10},
        bounds = {{"left", ".5"}, {"bottom", ".35"}, ".5", ".35"},
        text = getValue(v.values[2]),
        listeners = {
          enterPressed = apply
        }
      },
      TextInput {
        id = "3",
        font = {"Vera mono", 10},
        bounds = {"left", "bottom", ".5", ".35"},
        text = getValue(v.values[3]),
        listeners = {
          enterPressed = apply
        }
      },
      TextInput {
        id = "4",
        font = {"Vera mono", 10},
        bounds = {{"left", ".5"}, "bottom", ".5", ".35"},
        text = getValue(v.values[4]),
        listeners = {
          enterPressed = apply
        }
      }
    }
  }
end

Editors["lost.guiro.view.View"] = function(v, c)
  return View {
    bounds = {0, 0, "1", "1"},
    subviews = {
      Editors.get(v:bounds(), function(b)
        v:bounds(b)
      end)
    }
  }
end
