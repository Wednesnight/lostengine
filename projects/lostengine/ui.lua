local Bounds = lost.guiro.Bounds
local xabs = lost.guiro.xabs
local yabs = lost.guiro.yabs
local xrel = lost.guiro.xrel
local yrel = lost.guiro.yrel
local xright = lost.guiro.xright
local xleft = lost.guiro.xleft
local ytop = lost.guiro.ytop
local ybottom = lost.guiro.ybottom
local xcenter = lost.guiro.xcenter
local ycenter = lost.guiro.ycenter
local wabs = lost.guiro.wabs
local habs = lost.guiro.habs
local wrel = lost.guiro.wrel
local hrel = lost.guiro.hrel
local Rect = lost.math.Rect
local Vec2 = lost.math.Vec2
local Vec3 = lost.math.Vec3
local Color = lost.common.Color
local MatrixTranslation = lost.math.MatrixTranslation
local Loader = lost.resource.Loader
local FilesystemRepository = lost.resource.FilesystemRepository
local ApplicationResourceRepository = lost.resource.ApplicationResourceRepository
local SpawnTaskletEvent = lost.application.SpawnTaskletEvent

local c = 254/255
local labelHeight = 128

return dcl.guiro:Screen
{
  listeners = 
  {
    droppedFile = function(event)
      local labelStack = event.target:screen()("ui")("window")("content")("contentView")("labelStack")

      -- remove startupLabel
      if labelStack ~= nil and labelStack("startupLabel") ~= nil then
        labelStack:removeSubview(labelStack("startupLabel"))
      end
      
      -- add new label if tasklet doesn't exist
      if labelStack ~= nil and labelStack(event.filename) == nil then
        local newLabel = dcl.guiro:Label
        {
          id = event.filename,
          bounds = Bounds(xcenter(), ycenter(), wrel(1), habs(labelHeight)),
          text = event.filename,
          backgroundColor = Color(1,0,0,1),
          listeners =
          {
            mouseEnter = function(event) event.target:showBackground(true) end,
            mouseLeave = function(event) event.target:showBackground(false) end,
            mouseDown = function(event)
              local loader = Loader()
              loader:addRepository(FilesystemRepository.create(event.target.id))
              loader:addRepository(ApplicationResourceRepository.create())
              lostengineTasklet:dispatchApplicationEvent(SpawnTaskletEvent(loader))
            end
          }
        }
        labelStack:addSubview(newLabel)
        labelStack.bounds.height = habs(#labelStack.subviews * labelHeight)
        labelStack:needsLayout()
        labelStack.parent.bounds.height = labelStack.bounds.height
        labelStack.parent:needsLayout()
        labelStack.parent.parent:needsLayout()
      end
    end
  },

  id = "screen",
  dcl.guiro:UserInterface
  {
    id = "ui",
    bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
    dcl.guiro:Window
    {
      id = "window",
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      dcl.guiro:ScrollView
      {
        id = "content",
        bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
        contentViewBounds = Bounds(xleft(), ytop(), wabs(400), habs(128)),
        dcl.guiro:VBox
        {
          id = "labelStack",
          bounds = Bounds(xleft(), ytop(), wabs(400), habs(128)),
          halign = "center",
          valign = "center",
          mode = "spread",
          dcl.guiro:Label
          {
            id = "startupLabel",
            bounds = Bounds(xcenter(), ycenter(), wrel(1), hrel(1)),
            text = "Drop tasklet directory to get started!"
          }
        }
      }
    }
  }
}
