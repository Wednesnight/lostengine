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
            
          },
          dataSource = { -- programmatic interface to an arbitrary 
            data = listviewContent,
            
            numberOfSections=function(self) return #(self.data.sections) end, -- number of currently available number of sections, defaults to 1 if not specified
            numberOfRowsInSection=function(self,sectionIndex) return #((self.data.sections[sectionIndex]).items) end, -- number of currently available rows in given section,mandatory
            sectionInfoForIndex=function(self,sectionIndex) return self.data.sections[sectionIndex] end, -- doesn't have to be the actual section table, but return a table that contains everything you need for visualization of this section,optional
            rowForIndexPath=function(self,indexPath) return (self.data.sections[indexPath[1]]).items[indexPath[2]] end, -- indexPath = table with two 1-based indices into the actual content, returns the actual item that needs to be displayed,mandatory
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
