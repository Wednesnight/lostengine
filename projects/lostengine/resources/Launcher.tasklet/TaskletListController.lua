module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "TaskletListController" {}

function TaskletListController:constructor()
  self.view = require("ui_tasklets")
  self.view:dataSource(self)
  self.view:delegate(self)
  
  self._tasklets = {
    {
      path = "/Users/tony/hello",
      name = "test1"
    },
    {
      path = "/Users/tony/hello",
      name = "test2"
    }
  }
  self.view:reloadData()
end

-- listview delegate start

function TaskletListController:numberOfSections(listView) return 1 end -- number of currently available number of sections, defaults to 1 if not specified
function TaskletListController:numberOfRowsInSection(listView,sectionIndex) return #(self._tasklets) end -- number of currently available rows in given section,mandatory
function TaskletListController:rowForIndexPath(listView,indexPath) return self._tasklets[indexPath[2]] end -- indexPath = table with two 1-based indices into the actual content, returns the actual item that needs to be displayed,mandatory

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
    subviews=
    {
      lost.guiro.view.Label
      {
        text="hello",
      }
    },
    sublayers=
    {
      lost.guiro.layer.Rect{color=Color(1,0,0)},
      lost.guiro.layer.HLine{bounds={0,"bottom","1",1},color=Color(1,1,1)}
    }
  }
  
  result.dataSource = function(self,ds)
    self._dataSource = ds
  end
  result.reuseId = "cell"
  return result
end

