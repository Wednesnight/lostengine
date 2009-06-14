-- lost.font.Text
module("lost.io", package.seeall)

require("lost.io.Loader")

-- render the given text with the given size and font
-- all three parameters are requireed or an error is raised
function Loader:Text(definition)
  if (definition.text == nil) or
     (definition.size == nil) or
     (definition.font == nil) then
     error("text, definition and size are required",2)
  end   
  local result = definition.font:render(definition.text, definition.size)
  if definition.transform ~= nil then
    result.modelTransform = definition.transform
  end
  return result
end
