module("lost.io", package.seeall)

--[[
     lost.io.Loader extension: Screen
  ]]

require("lost.io.Loader")
require("lost.guiro.Screen")

function Loader:loadScreen(filename)
  log.debug("Loader:loadScreen()")
  
  -- holds the screen object
  local result = {}
  
  -- create screen object and set result
  self.Screen = function(self, definition) result = lost.guiro.Screen(definition) return result end

  -- copy global environment and map env.guiro to self
  local env = {}
  for k,v in next,_G do
    env[k] = v
  end
  env.guiro = self

  -- execute screen definition
  self:executeScript(filename, env)

  -- trigger hierarchy initialization
  result:initialize()

  return result
end
