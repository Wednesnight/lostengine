t = 
{
  color = 
  {
    1,1,1
  },
  alpha = 1,
  name = "Hello",
}

-- data must be table
function ser(data,isRoot)
  if isRoot then
    print("return")
  end
  print("{")
  for k,v in pairs(data) do
    if type(v) ~= "table" then
      print(tostring(k).." = "..tostring(v).." ,")
    else
      print(tostring(k).." = ")
      ser(v,false)
      print(",")
    end
  end
  print("}")
end

ser(t,true)