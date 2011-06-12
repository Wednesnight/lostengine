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

function TaskletListController:createCellView()
  local result = lost.guiro.view.View
  {
    id="cell",
    clip=true,
    subviews=
    {
      lost.guiro.view.Label
      {
        id="name",
        bounds={20,{"top",-10},{"1",-20},15},
        color=Color(0,0,0),
        font={"Vera",12},
        text="Hello",
        halign="left"
      },
      lost.guiro.view.Label
      {
        id="path",
        bounds={10,6,{"1",-20},10},
        text="hello",
        color=Color(.6,.6,.6),
        font = {"Vera",9},
        breakMode="char",
        halign="left"
      }
    },
    sublayers=
    {
      lost.guiro.layer.Rect{color=Color(1,1,1,.7),gradient="rrbg"},
      lost.guiro.layer.HLine{bounds={0,"bottom","1",1},color=Color(1,1,1,.7)}
    }
  }
  result.pathLabel = result:recursiveFindById("path")
  result.nameLabel = result:recursiveFindById("name")
  result.dataSource = function(self,ds)
    self._dataSource = ds
    self.pathLabel:text(ds.path)
    self.nameLabel:text(ds.name)
  end
  result.reuseId = "cell"
  return result
end

