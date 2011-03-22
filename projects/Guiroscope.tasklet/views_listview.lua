local Color = lost.common.Color

local loader = lost.resource.Loader.create()
loader:addRepository(lost.resource.FilesystemRepository.create("/"))

local datasource = {}
for name in loader:directory("") do
  table.insert(datasource, {name = name, path = tostring(name), padding = 0})
end

local directoryImage = lost.bitmap.Bitmap.create(tasklet.loader:load("img/blue-folder-horizontal.png"))
directoryImage:premultiplyAlpha()
directoryImage = lost.gl.Texture.create(directoryImage, lost.gl.Texture.Params())
local directoryOpenImage = lost.bitmap.Bitmap.create(tasklet.loader:load("img/blue-folder-horizontal-open.png"))
directoryOpenImage:premultiplyAlpha()
directoryOpenImage = lost.gl.Texture.create(directoryOpenImage, lost.gl.Texture.Params())
local rightImage = lost.bitmap.Bitmap.create(tasklet.loader:load("img/control-right.png"))
rightImage:premultiplyAlpha()
rightImage = lost.gl.Texture.create(rightImage, lost.gl.Texture.Params())
local downImage = lost.bitmap.Bitmap.create(tasklet.loader:load("img/control-down.png"))
downImage:premultiplyAlpha()
downImage = lost.gl.Texture.create(downImage, lost.gl.Texture.Params())
local documentImage = lost.bitmap.Bitmap.create(tasklet.loader:load("img/document.png"))
documentImage:premultiplyAlpha()
documentImage = lost.gl.Texture.create(documentImage, lost.gl.Texture.Params())

return lost.guiro.view.View
{
  sublayers =
  {
    lost.guiro.layer.Rect
    {
      filled = true,
      color = Color(1,1,1)
    }
  },
  subviews =
  {
    lost.guiro.view.ListView
    {
      alwaysShowCorner = true,
      delegate =
      {
        createCellView = function(self,indexPath)
          local label = lost.guiro.view.Label {
            bounds = {35,"center","1","1"},
            font = {"Vera", 11},
            text = "",
            color = Color(0,0,0),
            halign = "left"
          }
          local cell = lost.guiro.view.View {subviews = {label}}
          cell.label = label
          cell.images = images
          cell.dataSource = function(self,ds)
            if self.images == nil then
              if boost.filesystem.is_directory(ds.name:status()) then
                self.images = {
                  control = {
                    closed = lost.guiro.view.Image {
                      bounds = {ds.padding,"center",16,16},
                      flip = true,
                      texture = rightImage,
                      hidden = ds.data ~= nil
                    },
                    open = lost.guiro.view.Image {
                      bounds = {ds.padding,"center",16,16},
                      flip = true,
                      texture = downImage,
                      hidden = ds.data == nil
                    }
                  },
                  directory = {
                    closed = lost.guiro.view.Image {
                      bounds = {ds.padding+16,"center",16,16},
                      flip = true,
                      texture = directoryImage,
                      hidden = ds.data ~= nil
                    },
                    open = lost.guiro.view.Image {
                      bounds = {ds.padding+16,"center",16,16},
                      flip = true,
                      texture = directoryOpenImage,
                      hidden = ds.data == nil
                    }
                  }
                }
                self:add({self.images.control.closed, self.images.control.open, self.images.directory.closed, self.images.directory.open})
                self.images.control.closed:addEventListener("mouseDown", function(event) self:toggle(ds) end)
                self.images.control.open:addEventListener("mouseDown", function(event) self:toggle(ds) end)
              else
                self.images = {
                  file = lost.guiro.view.Image {
                    bounds = {ds.padding+16,"center",16,16},
                    flip = true,
                    texture = documentImage
                  }
                }
                self:addSubview(self.images.file)
              end
            end
            self.label:bounds(lost.guiro.Bounds(unpack({ds.padding+35,"center","1","1"})))
            self.label:text(tostring(ds.name))
          end
          cell.toggle = function(self,ds)
            if ds.data ~= nil then
              ds.data = nil
              self:superview():superview():reloadData()
            else
              ds.data = {}
              for name in loader:directory(ds.path) do
                table.insert(ds.data, {name = name, path = tostring(boost.filesystem.path(ds.path) / tostring(name)), padding = ds.padding+25})
              end
              self:superview():superview():reloadData()
            end
          end
          cell.reuseId = "cell"..indexPath[2]
          return cell
        end,
        cellForRowAtIndexPath   = function(self,listView,indexPath) 
            local result = listView:dequeueCell("cell"..indexPath[2])
            if result == nil then
                result = self:createCellView(indexPath) 
            end
            return result
          end,
        heightForRowAtIndexPath = function(self,listView,indexPath) return 20 end
      },
      dataSource = {
        data = datasource,
        getCount = function(self,data)
          local result = 0
          for k,v in ipairs(data) do
            result = result+1
            if v.data ~= nil then
              result = result + self:getCount(v.data)
            end
          end
          return result
        end,
        numberOfRowsInSection = function(self,listView,sectionIndex) return self:getCount(self.data) end,
        getData = function(self,data,index,current)
          local result = nil
          for k,v in ipairs(data) do
            if current == index then
              result = v
            else
              current = current+1
              if v.data ~= nil then
                result, current = self:getData(v.data, index, current)
              end
            end
            if result ~= nil then
              break
            end
          end
          return result, current
        end,
        rowForIndexPath = function(self,listViewuitable,indexPath) return self:getData(self.data, indexPath[2], 1) end
      }
    }
  }
}