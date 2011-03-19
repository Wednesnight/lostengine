-- sbtest
local Vec2 = lost.math.Vec2
local Color = lost.common.Color

local listviewContent = 
{
  sections=
  {
    {
      title="First Section",
      items=
      {
        {title="1"},
        {title="2"},
        {title="2.5"},
      }
    },
    {
      title="Second Section",      
      items={
        {title="3"},
        {title="4"},        
        {title="4.5"},        
        {title="4.7"},        
      }
    },
    {
      title="Third Section",      
      items={
        {title="5"},
        {title="5.5"},
        {title="6.5"},        
        {title="7"},        
        {title="8"},        
      }
    }
  }
}

function startup()
  require("lost/guiro")
  lost.guiro.ui():add
  {
    lost.guiro.view.View
    {
      subviews=
      {
        lost.guiro.view.ListView
        {
          alwaysShowCorner=true,
          delegate = {
            
            createHeaderView=function(self)
              local c = .8
              local v = lost.guiro.view.View
              {
                sublayers={
                  lost.guiro.layer.Rect{filled=true,color=Color(c,c,c),gradient="candyGray"}
                },
              }
              local l = lost.guiro.view.Label
              {
                text="Header",
                color=Color(0,0,0),
                shadow=true,
                shadowColor=Color(1,1,1),
                shadowOffset=Vec2(0,-1)
              }
              v:addSubview(l)
              v.label = l
              v.dataSource = function(self,ds)
                self.label:text(ds.title)
              end
              v.reuseId = "header"
              return v
            end,
            
            createCellView=function(self)
              local backlayer = lost.guiro.layer.Rect{filled=true,color=Color(.6,.6,.9)}
              local v = lost.guiro.view.View
              {
                sublayers={
                  backlayer,
                },
              }
              v.backlayer = backlayer
              v.switchColor = function(self,flag)
                if flag then 
                  self.backlayer:color(Color(1,1,1))
                else
                  self.backlayer:color(Color(237/255,243/255,254/255))
                end
              end
              local l = lost.guiro.view.Label
              {
                text="Cell",
                bounds={0,"center",100,30},
                color=Color(0,0,0),
                font={"Grinched",16},
                valign="center",
                halign="center"
              }
              local b = lost.guiro.view.Button
              {
                title="Click!",
                bounds={"right","center",100,30},
              }
              v.label = l
              v.button = b
              v:addSubview(l)
              v:addSubview(b)
              v.dataSource = function(self,ds)
--                log.debug("row "..tostring(ds))
--                for a,b in pairs(ds) do
--                  log.debug(tostring(a).." "..tostring(b))
--                end
  --              log.debug("setting title: '"..tostring(ds.title).."'")
                self._dataSource = ds
                self.label:text(ds.title)
                self.button:title(ds.title)
              end
              v.buttonClick=function(self,event)
                log.debug("clicked button "..self._dataSource.title)
              end
              v.button:addEventListener("buttonClick",function(event) v:buttonClick(event) end)
              v.reuseId = "cell"
              return v
            end,
            
            viewForHeaderInSection=function(self,listView,sectionIndex) 
                local result = listView:dequeueCell("header")
                if result == nil then
                  log.debug("creating header")
                  result = self:createHeaderView()
                end
                return result
              end, -- optional, header will be omitted if nil is returned
            heightForHeaderInSection=function(self,listView,sectionIndex) return 20 end, -- optional, defaults to something
            cellForRowAtIndexPath=function(self,listView,indexPath) 
                local result = listView:dequeueCell("cell")
                if result == nil then
                  log.debug("creating cell")
                  result = self:createCellView()
                end
                result:switchColor((indexPath[2] % 2) == 0)
                return result
              end, -- mandatory, must never return nil
            heightForRowAtIndexPath=function(self,listView,indexPath) return 40 end, -- optional, defaults to something
          },
          dataSource = {  
            data = listviewContent,
            
            numberOfSections=function(self,listView) return #(self.data.sections) end, -- number of currently available number of sections, defaults to 1 if not specified
            numberOfRowsInSection=function(self,listView,sectionIndex) return #((self.data.sections[sectionIndex]).items) end, -- number of currently available rows in given section,mandatory
            sectionInfoForIndex=function(self,listView,sectionIndex) return self.data.sections[sectionIndex] end, -- doesn't have to be the actual section table, but return a table that contains everything you need for visualization of this section,optional
            rowForIndexPath=function(self,listViewuitable,indexPath) return (self.data.sections[indexPath[1]]).items[indexPath[2]] end, -- indexPath = table with two 1-based indices into the actual content, returns the actual item that needs to be displayed,mandatory
          }
        }
      }
    }
  }
end

function update()
--  lost.guiro.ui().layer:print()
--  lost.guiro.ui():printSubviews()
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
