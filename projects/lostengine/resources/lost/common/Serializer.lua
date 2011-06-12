module("lost.common", package.seeall)

require("lost.common.Class")

lost.common.Class "lost.common.Serializer" {}

function Serializer:constructor()
  self._buffer = ""
  self._indentString = "  "
  self._indents = {}  
end

local function isPositiveInteger(n)
  return type(n) == "number" and n > 0 and math.floor(n) == n
end

local function isSequential(array, max)
  for i=1, max do
    if array[i] == nil then return false end
  end
  return true
end

local function isArray(array)
  local max = 0
  for k, _ in pairs(array) do
    if not isPositiveInteger(k) then return false end
    max = math.max(max, k)
  end
  return isSequential(array, max)
end

function Serializer:indent(n)
  local result = self._indents[n]
  if not result then
    result = ""
    for i=1,n do
      result = result .. self._indentString
    end
    self._indents[n] = result
  end
  return result
end

function Serializer:append(str,nindent)
  self._buffer = self._buffer .. self:indent(nindent)..str.."\n"
end

function Serializer:_serialize(data,nindent,isRoot)
  local n = nindent
  if isRoot then
    self:append("return",n)
  end
  self:append("{",n)
  
  local ia = isArray(data)
  
  if ia then
    for k,v in pairs(data) do
      if type(v) ~= "table" then
        self:append(tostring(v)..",",n+1)
      else
        self:_serialize(v,n+1,false)
        self:append(",",n+1)
      end
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
        self:append(tostring(k).." = "..vs..",",n+1)
      else
        self:append(tostring(k).." = ",n+1)
        self:_serialize(v,n+1,false)
        self:append(",",n+1)
      end
    end
  end
  self:append("}",n)
end

function Serializer:serialize(inTable)
  self._buffer = ""
  self:_serialize(inTable,0,true)
  return self._buffer
end

