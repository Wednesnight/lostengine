module("lost.io", package.seeall)

require("lost.io.Loader")
require("lost.io.rg.Node")


-- This is the root loader function for scenes defined/constructed via rg.
-- rg:Scene creates the function environment and makes sure that the result
-- of the construction can be resturned from loadScene. This is also the 
-- reason why self.Scene is defined inline and not externally: this way, "result"
-- can be taken form the loadScene function context and reused within 
-- self.Scene.
-- Scene is loaded with the usual function environment magic, but is only a 
-- Node apart from that, no special attributes allowed
function Loader:loadScene(filename)
  log.debug("Loader:loadScene()")
  
  -- holds the scene object
  local result = {}
  
  -- create screen object and set result
  self.Scene = function(self, definition) result = self:Node(definition) return result end

  -- copy global environment and map env.guiro to self
  local env = {}
  for k,v in next,_G do
    env[k] = v
  end
  env.lostrg = self
  env.lostmesh = self -- FIXME we need to pull these subloaders apart
  env.lostgl = self -- FIXME we need to pull these subloaders apart
  env.lostresource = self -- FIXME we need to pull these subloaders apart

  -- execute screen definition
  self:executeScript(filename, env)

  return result
end
