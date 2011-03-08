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
          delegate = {
            
            createHeaderView=function(self)
              return lost.guiro.view.View
              {
                sublayers={
                  lost.guiro.layer.Rect{filled=true,color=Color(0,0,0)}
                },
                subviews=
                {
                  lost.guiro.view.Label
                  {
                    text="Header",
                    color=Color(1,1,1),
                  }
                }
              }
            end,
            
            createCellView=function(self)
              return lost.guiro.view.View
              {
                sublayers={
                  lost.guiro.layer.Rect{filled=true,color=Color(.6,.6,.9)},
                  lost.guiro.layer.HLine{bounds={0,"bottom","1",1},color=Color(.8,.8,.8)}
                },
                subviews=
                {
                  lost.guiro.view.Label
                  {
                    text="Cell",
                    color=Color(1,1,1),
                    font={"Grinched",16}
                  }
                }
              }
            end,
            
            viewForHeaderInSection=function(self,listView,sectionIndex) return self:createHeaderView() end, -- optional, header will be omitted if nil is returned
            heightForHeaderInSection=function(self,listView,sectionIndex) return 20 end, -- optional, defaults to something
            cellForRowAtIndexPath=function(self,listView,indexPath) return self:createCellView() end, -- mandatory, must never return nil
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
