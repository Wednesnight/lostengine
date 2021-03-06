require("lost.common.Class")
require("FileListController")

lost.common.Class "MainController" {}

function MainController:constructor()
  self.files = {}
  self.selectedFile = 0
  self.fileListController = FileListController()
end

function MainController:buildUi()
  lost.guiro.ui():add{
    lost.guiro.view.View{
      style="gray",
      subviews={
        lost.guiro.view.TabView
        {
          style="square",
          items=
          {
            {"Files",self.fileListController:view()},
            {"Atlas",lost.guiro.view.View{}},
          }
        }    
      }
    }
  }
  lost.guiro.ui():addEventListener("drop",function(event) self:fileDropped(event) end)

end

function MainController:fileDropped(event)
  self.fileListController:receivedDropEvent(event)
end

