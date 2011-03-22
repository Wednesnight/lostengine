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
  local pathLabelHeight = 20
  local titleLabelHeight = 20
  local pathLabelSpacing=4
  
  self.bgcol1 = Color(1,1,1)
  self.bgcol2 = Color(237/255,243/255,254/255)
  self.titleLabelSelectedColor = Color(1,1,1)
  self.titleLabelDeselectedColor = Color(0,0,0)
  self.pathLabelSelectedColor = Color(.9,.8,1)
  self.pathLabelDeselectedColor = Color(.5,.5,.5)
  self.numberLabelDeselectedColor = Color(.2,.4,1)
  self.numberLabelSelectedColor = Color(1,1,1)
  self.bgcolSelected = Color(.2,.4,1)
  self.bglayer = lost.guiro.layer.Rect{filled=true}
  self.layer:addSublayer(self.bglayer)
  
  self.imageView = lost.guiro.view.Image
  {
    bounds={margin,margin,30,{"1",-(2*margin)}},
    scale="aspect",
    valign="center",
    halign="center",
    filter=true
  }
  self:addSubview(self.imageView)
  self.titleLabel=lost.guiro.view.Label
  {
    bounds={margin+imageWidth+imageSpacing,{"top",-margin},{"1",-(2*margin+imageWidth+imageSpacing)},titleLabelHeight},
    text="Title",
    valign="top",
    halign="left",
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
    color=Color(.5,.5,.5),
    clip=true,
    valign="center",
    halign="left",
    breakMode="char"
  }
  self:addSubview(self.pathLabel)
  
  self.bitmapNumberLabel = lost.guiro.view.Label
  {
    bounds={{"right",-margin},{"top",-margin},40,titleLabelHeight},
    cli=true,
    color=Color(.2,.4,1),
    font={"Vera",12},
    halign="center",
    valign="top",
  }
  self:addSubview(self.bitmapNumberLabel)
  
  self.removeButton = lost.guiro.view.Button
  {
    title="R",
    bounds={"left","top",20,20},
  }
  self:addSubview(self.removeButton)
  
  self._switchColor = true
  self._selected = false
end

function FileListCell:mouseDown(event)
  self.delegate:cellSelected(self)
end

function FileListCell:update()
  lost.guiro.view.View.update(self)
    
  if not self._selected then
    if self._switchColor then
      self.bglayer:color(self.bgcol1)
    else
      self.bglayer:color(self.bgcol2)
    end
    self.titleLabel:color(self.titleLabelDeselectedColor)
    self.bitmapNumberLabel:color(self.numberLabelDeselectedColor)
    self.pathLabel:color(self.pathLabelDeselectedColor)
  else
    self.bglayer:color(self.bgcolSelected)
    self.titleLabel:color(self.titleLabelSelectedColor)
    self.bitmapNumberLabel:color(self.numberLabelSelectedColor)
    self.pathLabel:color(self.pathLabelSelectedColor)
  end
end

function FileListCell:switchColor(...)
  if arg.n >= 1 then
    self._switchColor = arg[1]
    self:needsUpdate()
  else
    return self._switchColor
  end
end

function FileListCell:select(...)
  if arg.n >= 1 then
    self._selected = arg[1]
    self:needsUpdate()
  else
    return self._selected
  end
end

function FileListCell:dataSource(ds)
  self.titleLabel:text(ds.filename)
  self.imageView:texture(ds.texture)
  self.pathLabel:text(ds.path)
  if ds.isGif and (ds.numBitmaps > 1) then
    self.bitmapNumberLabel:text("("..tostring(ds.numBitmaps)..")")
  else
    self.bitmapNumberLabel:text(nil)
  end
end

