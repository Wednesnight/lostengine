require("lost.common.Class")

lost.common.Class "FileListCell" "lost.guiro.view.View" {}

local Color = lost.common.Color

FileListCell.reuseId = "fileListCell"

function FileListCell:constructor(args)
  lost.guiro.view.View.constructor(self,args)
  self.id="filelistcell"
  
  local margin = 4
  local imageWidth = 30
  local imageSpacing = 10
  local pathLabelHeight = 15
  local pathLabelSpacing=4
  
  self.bgcol1 = Color(1,1,1)
  self.bgcol2 = Color(237/255,243/255,254/255)
  self.bglayer = lost.guiro.layer.Rect{filled=true}
  self.layer:addSublayer(self.bglayer)
  
  self.imageView = lost.guiro.view.Image
  {
    bounds={margin,margin,30,{"1",-(2*margin)}},
    scale="aspect",
    valign="center",
    halign="center",
  }
  self:addSubview(self.imageView)
  self.titleLabel=lost.guiro.view.Label
  {
    bounds={margin+imageWidth+imageSpacing,margin+pathLabelHeight+pathLabelSpacing,{"1",-(2*margin+imageWidth+imageSpacing)},pathLabelHeight},
    text="Title",
    font={"Vera",12},
    color=Color(0,0,0),
    clip=true,
  }
  self:addSubview(self.titleLabel)  
  
  self.pathLabel = lost.guiro.view.Label
  {
    bounds={margin+imageWidth+imageSpacing,margin,{"1",-(2*margin+imageWidth+imageSpacing)},pathLabelHeight},
    text="path",
    font={"Vera",8},
    color=Color(.4,.4,.4),
    clip=true,
    valign="center",
    halign="left",
    breakMode="char"
  }
  self:addSubview(self.pathLabel)
end

function FileListCell:switchColor(flag)
  if flag then
    self.bglayer:color(self.bgcol1)
  else
    self.bglayer:color(self.bgcol2)
  end
end

function FileListCell:dataSource(ds)
  self.titleLabel:text(ds.filename)
  self.imageView:texture(ds.texture)
  self.pathLabel:text(ds.path)
end