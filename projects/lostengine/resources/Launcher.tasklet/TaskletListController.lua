module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "TaskletListController" {}

function TaskletListController:constructor()
  self.view = require("ui_tasklets")
  self._listView = self.view:recursiveFindById("listview")
  self._listView:dataSource(self)
  self._listView:delegate(self)
  
  self._tasklets = {}
  self._listView:reloadData()
end

function TaskletListController:tasklets(...)
  if arg.n >= 1 then
    self._tasklets = arg[1]
    self._listView:reloadData()
  else
    return self._tasklets
  end
end
-- listview delegate start

function TaskletListController:numberOfSections(listView) return 1 end -- number of currently available number of sections, defaults to 1 if not specified
function TaskletListController:numberOfRowsInSection(listView,sectionIndex) return #(self._tasklets) end -- number of currently available rows in given section,mandatory
function TaskletListController:rowForIndexPath(listView,indexPath) return self._tasklets[indexPath[2]] end -- indexPath = table with two 1-based indices into the actual content, returns the actual item that needs to be displayed,mandatory
function TaskletListController:heightForRowAtIndexPath(indexPath)
  return 60
end

function TaskletListController:cellForRowAtIndexPath(listView,indexPath)  -- mandatory, must never return nil
--  log.debug("----------------")
  local result = listView:dequeueCell("cell")
  if result == nil then
--    log.debug("creating cell")
    result = self:createCellView()
  end
--  log.debug("returning object of type "..type(result))
  return result
end

-- listview delegate end

function TaskletListController:playTexture()
  if not self._playTexture then
    local bmp = lost.bitmap.Bitmap.create(tasklet.loader:load("131.png"))
    bmp:premultiplyAlpha()
    local texparam = lost.gl.Texture.Params()
    self._playTexture = lost.gl.Texture.create(bmp, texparam)
  end
  return self._playTexture
end

function TaskletListController:taskletStartRequested(entry)
  log.debug("start "..entry.path)
  tasklet:dispatchApplicationEvent(lost.application.SpawnTaskletEvent.create(entry.path))  
end

function TaskletListController:createCellView()
  local result = lost.guiro.view.View
  {
    id="cell",
    clip=true,
    listeners=
    {
      buttonClick=function(event) 
        if event.target.id == "autorun" then
          log.debug("autorun "..tostring(event.target:pushed())) 
          event.currentTarget:dataSource().autorun = event.target:pushed()
        end
      end,
      mouseDown=function(event) 
        if (event.target.id == "start") or (event.target.id == "play") or (event.target.id=="name") then
          self:taskletStartRequested(event.currentTarget:dataSource())
        end
      end
    },    
    subviews=
    {
      lost.guiro.view.View
      {
        id="start",
        bounds={0,0,{"1",-64},"1"},
        clip=true,
        subviews=
        {
          lost.guiro.view.Image
          {
            id="play",
            texture=self:playTexture(),
            bounds={0,0,20,"1"},
            valign="center",
            halign="center"
          },
          lost.guiro.view.Label
          {
            id="name",
            bounds={24,"center",{"1",-24},15},
            color=Color(0,0,0),
            font={"Vera",12},
            text="Hello",
            halign="left"
          },
        },
        sublayers=
        {
          lost.guiro.layer.Rect{bounds={0,0,"1","1"},color=Color(1,1,1,.7),gradient="rrbg"},
        }
      },
      lost.guiro.view.Button
      {
        id="autorun",
        bounds={"right",{"bottom",8},60,10},
        style="checkboxCandy",
        size="mini",
        title="Autorun"
      },
      lost.guiro.view.Button
      {
        listeners=
        {
          buttonClick=function(event) log.debug("remove") end
        },
        id="remove",
        theme="launcher",
        style="remove",
        bounds={{"right",-4},{"top",-8},10,10},
      }
    },
    sublayers=
    {
      lost.guiro.layer.Rect{bounds={"right",0,64,"1"},color=Color(.7,.7,.7,.7),gradient="rrbg"},
      lost.guiro.layer.HLine{bounds={0,"bottom","1",1},color=Color(1,1,1,.7)}
    }
  }
  result.nameLabel = result:recursiveFindById("name")
  result.autorunCheckbox = result:recursiveFindById("autorun")
  result.dataSource = function(self,ds)
    if ds ~= nil then
      self._dataSource = ds
      self.nameLabel:text(ds.name)
      self.autorunCheckbox:pushed(ds.autorun)
    else
      return self._dataSource
    end
  end
  result.reuseId = "cell"
  return result
end

