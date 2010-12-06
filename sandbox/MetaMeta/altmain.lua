log.debug("hello")

require("lost.declarative.Context")

using "lost.math.Rect"
using "lost.guiro.Bounds"
using "lost.guiro.xabs"
using "lost.guiro.yabs"
using "lost.guiro.xrel"
using "lost.guiro.yrel"
using "lost.guiro.xright"
using "lost.guiro.xleft"
using "lost.guiro.ytop"
using "lost.guiro.ybottom"
using "lost.guiro.xcenter"
using "lost.guiro.ycenter"
using "lost.guiro.wabs"
using "lost.guiro.habs"
using "lost.guiro.wrel"
using "lost.guiro.hrel"
using "lost.common.Color"
using "lost.math.Rect"
using "lost.math.Vec2"
using "lost.math.Vec3"

rootNode = nil
dcl = nil
screen = nil

function startup()
  log.debug("startup")
  dcl = lost.declarative.Context(tasklet.loader)
--[[  rootNode = dcl.rg:Node
  {
    id = "rootNode",
    dcl.rg:Camera2D
    {
      viewport = Rect(0,0,320,480)
    },
    dcl.rg:ClearColor
    {
      color = Color(1,0,0,1)
    },
    dcl.rg:Clear
    {
      mask = gl.GL_COLOR_BUFFER_BIT
    },
    dcl.rg:Draw
    {
      id = "quadDraw",
      mesh = dcl.mesh:Quad
      {
        rect = Rect(10,10,100,100),
        material = 
        {
          color = Color(1,1,0,1),
        }
      }
    }
  }
  tasklet.renderNode:add(rootNode)]]
  screen = dcl.guiro:Screen
  {
    id = "screen",
    dcl.guiro:UserInterface
    {
      id = "mainUi",
      bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
      dcl.guiro:Window
      {
        id = "window1",
        bounds = Bounds(xabs(0), yabs(0), wrel(1), hrel(1)),
        dcl.guiro:Image
        {
          id = "image1",
          bounds = Bounds(xabs(10), yabs(100), wabs(100), habs(75)),
          filename = "lost/guiro/themes/default/resources/ButtonHover.png",
          scale="scalegrid",
          showFrame = false,
          caps = 
          {
            left = 3,
            right=3,
            top=3,
            bottom=3
          },
          flip = true,
          filter = true,
        },
        dcl.guiro:Label
        {
          bounds = Bounds(xabs(10), yabs(200), wabs(100), habs(75)),
          text = "Hello World!"
        }
      }
    }
  }
  return true
end

function update()
  
  return true
end

function shutdown()
  log.debug("shutdown")
  return true
end