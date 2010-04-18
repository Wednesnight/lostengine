--[[
    lost.guiro.View
    
    View is the base class of all Guiro objects, it also implements all drawing related functionality.
    Example:

      lost.common.Class "MyDrawable" "lost.guiro.View" {}
      
      function MyDrawable:beforeRedraw()
        -- update meshes, colors, etc.
      end
      
      function MyDrawable:afterRedraw()
        -- update meshes, colors, etc.
      end
      
      function MyDrawable:modifyingMethod()
        -- modify members, colors, etc.
        self:needsRedraw()
      end
  ]]
module("lost.guiro", package.seeall)

require("lost.common.Class")
require("lost.guiro.HasLayout")
require("lost.guiro.HasSubviews")
require("lost.guiro.HasEvents")
require("lost.common.Shaders")

lost.common.Class "lost.guiro.View" "lost.guiro.HasLayout" "lost.guiro.HasSubviews" "lost.guiro.HasEvents"
{
  -- helper for auto-generated view ids
  indices = {}
}

using "lost.common.callLater"
using "lost.math.Vec2"
using "lost.math.Vec3"
using "lost.math.MatrixTranslation"

function View:constructor(textureManager)
  assert(textureManager, "View requires lost.guiro.TextureManager instance for construction")
  -- call interface ctors
  lost.guiro.HasLayout.constructor(self)
  lost.guiro.HasSubviews.constructor(self)
  lost.guiro.HasEvents.constructor(self)

  -- all views and derived classes receive a default id in the
  -- constructor so they can be uniquely identified
  self.id = self:createDefaultId()

  -- RenderGraph node
  self.rootNode = lost.rg.Node.create()
  self.rootNode.name = self.id
  self.renderNode = lost.rg.Node.create()
  self.renderNode.name = "renderNode"
  self.subviewNodes = lost.rg.Node.create()
  self.subviewNodes.name = "subviewNodes"

  -- scissoring, initially enabled but inactive (will not be processed)
  self.scissorBounds = lost.guiro.Bounds(lost.guiro.xleft(), lost.guiro.ytop(), lost.guiro.wrel(1), lost.guiro.hrel(1))
  self.scissorNode = lost.rg.Scissor.create(true)
  self.scissorNode.active = false
  self.scissorNode.name = "scissorNode"
  self.scissorRectNode = lost.rg.ScissorRect.create(lost.math.Rect())
  self.scissorRectNode.active = false
  self.scissorRectNode.name = "scissorRectNode"
  self.disableScissorNode = lost.rg.Scissor.create(false)
  self.disableScissorNode.active = false
  self.disableScissorNode.name = "disableScissorNode"

  -- set scissor rect to enable scissoring for this view
  self.scissorRect = nil

  -- draw the view
  self.rootNode:add(self.renderNode)
  -- apply the views scissoring
  self.rootNode:add(self.scissorNode)
  self.rootNode:add(self.scissorRectNode)
  -- draw subviews
  self.rootNode:add(self.subviewNodes)
  -- disable scissoring
  self.rootNode:add(self.disableScissorNode)

  -- meshes and draw nodes
  self.backgroundMesh = lost.mesh.RoundedRect.create(textureManager._textureManager, 
                                                     Vec2(self.rect.width, self.rect.height),
                                                     true,
                                                     8,
                                                     1)
  self.backgroundMesh:roundCorners(false, false, false, false)
  self.backgroundMesh.material.shader = lost.common.Shaders.textureShader()
  self.backgroundMesh.material.blend = true  
  self.backgroundMesh.material.color = lost.common.Color(1,0,0,1)
  self.backgroundNode = lost.rg.Draw.create(self.backgroundMesh)
  self.backgroundNode.name = "drawViewBackground"
  self.backgroundNode.active = false

  self.frameMesh = lost.mesh.RoundedRect.create(textureManager._textureManager,
                                                Vec2(self.rect.width, self.rect.height),
                                                false,
                                                8,
                                                1)
  self.frameMesh:roundCorners(false, false, false, false)                                                
  self.frameMesh.material.shader = lost.common.Shaders.textureShader()
  self.frameMesh.material.blend = true  
  self.frameMesh.material.color = lost.common.Color(1,1,1,1)
  self.frameNode = lost.rg.Draw.create(self.frameMesh)
  self.frameNode.name = "drawViewFrame"
  self.frameNode.active = false

  self.renderNode:add(self.backgroundNode)
  self.renderNode:add(self.frameNode)

  self.dirty = false
end

--[[
    Called by lost.guiro.HasSubviews when attaching this View to another
  ]]
function View:onAttach(parent)
  self:setParent(parent)
end

--[[
    Called by lost.guiro.HasSubviews when detaching this View from its parent
  ]]
function View:onDetach(parent)
  self:setParent(nil)
end

--[[
    Re-/set parent view
  ]]
function View:setParent(parent)
  if parent ~= nil then
    if parent:isDerivedFrom("lost.guiro.View") then
      self.parent = parent
    else
      local typeName = type(parent)
      if type(parent.className) == "function" then
        typeName = parent:className()
      end
      error("View:setParent() expected lost.guiro.View, got ".. typeName, 2)
    end
    self.parent.subviewNodes:add(self.rootNode)
    self:needsLayout()
    self:needsRedraw()
  else
    if self.parent ~= nil then
      self.parent.subviewNodes:remove(self.rootNode)
    end
    self.parent = parent
  end
end

--[[
    Get the root view of the view hierarchy
  ]]
function View:rootView()
  if self.parent ~= nil then
    return self.parent:rootView()
  else
    return self
  end
end

--[[ 
    prints self.subviews hierarchy
  ]]
function View:printSubviews(prefix)
  if not prefix then
    prefix = ""
  end
  log.debug(prefix .."|-- ".. self.id)
  for k,view in next,self.subviews do
    view:printSubviews(prefix .."    ")
  end
end

--[[
    Utility method for auto-generated ids
  ]]
function View:createDefaultId()
  local result = nil
  local name = self:className()
  if (not View.indices[name]) then
    View.indices[name] = 1
  else
    View.indices[name] = View.indices[name]+1
  end
  result = name .. View.indices[name]
  return result
end

--[[
    Updates dirty flag and inserts self into redraw queue
  ]]
function View:needsRedraw()
  if not self.dirty then
    self.dirty = true
    
    -- add to tasklet queue
    callLater(View._redraw, self)
  end
end

function View:afterLayout()
  lost.guiro.HasLayout.afterLayout(self)

  -- update background mesh
  self.backgroundMesh:size(Vec2(self.rect.width, self.rect.height))
  self.backgroundMesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))

  -- update frame mesh
  self.frameMesh:size(Vec2(self.rect.width, self.rect.height))
  self.frameMesh.transform = MatrixTranslation(Vec3(self.rect.x, self.rect.y, 0))

  -- update scissoring
  if self.scissorRect ~= nil then
    self.scissorRectNode.rect = self.scissorRect
  else
    self.scissorRectNode.rect = self.rect
  end
end

--[[
    Internal redraw, do not use! See also: View:beforeRedraw(), View:afterRedraw()
  ]]
function View:_redraw()
--  log.debug(tostring(self) .."(".. self.id .."):_redraw()")
  self:beforeRedraw()
  self.dirty = false

  -- Call invisible _redraw() on all subviews
  if type(self.subviews) == "table" then
    for k,view in next,self.subviews do
      if view:isDerivedFrom("lost.guiro.View") then
        view:_redraw(true)
      end
    end
  end

  self:afterRedraw()
end

--[[
    Override this method to implement your redraw code before base objects are updated
  ]]
function View:beforeRedraw()
end

--[[
    Override this method to implement your redraw code after base objects were updated
  ]]
function View:afterRedraw()
end

function View:showFrame(flag)
  if flag ~= nil then
    self.frameNode.active = flag
  else
    return self.frameNode.active
  end
end

function View:frameColor(col)
  if col ~= nil then
    self.frameMesh.material.color = col
  else
    return self.frameMesh.material.color
  end
end

function View:showBackground(flag)
  if flag ~= nil then
    self.backgroundNode.active = flag
  else
    return self.backgroundNode.active
  end
end

function View:backgroundColor(col)
  if col ~= nil then
    self.backgroundMesh.material.color = col
  else
    return self.backgroundMesh.material.color
  end
end

function View:hidden(val)
	if val ~= nil then
		self.rootNode.active = not val
	else
		return not self.rootNode.active
	end
end

--[[
    metatable methods
  ]]
function View:__tostring()
  return self:className() .."(".. self.id ..")"
end
