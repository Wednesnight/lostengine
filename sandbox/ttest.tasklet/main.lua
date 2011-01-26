
local Color=lost.common.Color
local Vec2=lost.math.Vec2

local t1 = 
[[
Hello world!
This is the second line.
And another one?!$%&
]]

local t2 = 
[[
Frankenstein,

or the Modern Prometheus


by

Mary Wollstonecraft (Godwin) Shelley




Letter 1


St. Petersburgh, Dec. 11th, 17--

TO Mrs. Saville, England

You will rejoice to hear that no disaster has accompanied the commencement of an enterprise which you have regarded with such evil forebodings.  I arrived here yesterday, and my first task is to assure my dear sister of my welfare and increasing confidence in the success of my undertaking.

I am already far north of London, and as I walk in the streets of Petersburgh, I feel a cold northern breeze play upon my cheeks, which braces my nerves and fills me with delight.  Do you understand this feeling?  This breeze, which has travelled from the regions towards which I am advancing, gives me a foretaste of those icy climes. Inspirited by this wind of promise, my daydreams become more fervent and vivid.
]]

function startup()
  require("lost.guiro")

  lost.guiro.ui():add
  {
    lost.guiro.view.Label
    {
      text=t2,
--      color=Color(1,1,1),
--      shadow=true,
      shadowColor=Color(1,0,0),
      shadowOffset=Vec2(1,-1)      
    }
  }
end

function key(event)
  if event.key == lost.application.K_ESCAPE then
    tasklet.running = false
  end
end
