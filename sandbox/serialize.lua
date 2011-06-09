t = 
{
  color = 
  {
    1,1,1
  },
  alpha = 1,
  name = "Hello",
}

local function isPositiveInteger(n)
  return type(n) == "number" and n > 0 and math.floor(n) == n
end

local function isSequential(array, max)
  for i=1, max do
    if array[i] == nil then return false end
  end
  return true
end

function isArray(array)
  local max = 0
  for k, _ in pairs(array) do
    if not isPositiveInteger(k) then return false end
    max = math.max(max, k)
  end
  return isSequential(array, max)
end

local _indentString = "  "
local _indents = {}

-- returns string that is n * _indentString. Results are cached
function indent(n)
  local result = _indents[n]
  if not result then
    result = ""
    for i=1,n do
      result = result .. _indentString
    end
    _indents[n] = result
  end
  return result
end

function append(str,nindent)
  print(indent(nindent)..str)
end

-- data must be table
function ser(data,nindent,isRoot)
  local n = nindent
  if isRoot then
    append("return",n)
  end
  append("{",n)
  
  local ia = isArray(data)
  
  if ia then
    for k,v in pairs(data) do
        append(tostring(v)..",",n+1)
    end
  else
    for k,v in pairs(data) do
      if type(v) ~= "table" then
        local vs = ""
        if type(v) == "string" then
          vs = "\""..v.."\""
        else
          vs = tostring(v)
        end
        append(tostring(k).." = "..vs..",",n+1)
      else
        append(tostring(k).." = ",n+1)
        ser(v,n+1,false)
        append(",",n+1)
      end
    end
  end
  append("}",n)
end

ser(t,0,true)