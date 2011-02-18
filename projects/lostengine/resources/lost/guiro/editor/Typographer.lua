module("lost.guiro.editor", package.seeall)

require("lost.common.Class")
require("lost.common.Utils")

lost.common.Class "lost.guiro.editor.Typographer" {}

local foreach = lost.common.foreach

function Typographer:constructor()
  self._ignore = nil
  self._recursionCount = -1
end

function Typographer:__call(obj)
  local result = nil
  local t = type(obj)
  self._recursionCount = self._recursionCount + 1
  if type(self._ignore) ~= "table" or not foreach(self._ignore, function(k,v) return rawequal(v,obj) end) then
    local printer = self[t] or self[t .."_"]
    if printer ~= nil then
      result = printer(self, obj)
    end
  end
  if self._recursionCount == 0 then
    self._ignore = nil
    self._recursionCount = -1
  else
    self._recursionCount = self._recursionCount - 1
  end
  return result or string.format("[%s]", t)
end

function Typographer:ignore(obj)
  if self._ignore == nil then
    self._ignore = {}
  end
  table.insert(self._ignore, obj)
end

function Typographer:_defaultType(t)
  return tostring(t)
end

function Typographer:nil_(n)
  return self:_defaultType(n)
end

function Typographer:number(n)
  return self:_defaultType(n)
end

function Typographer:string(s)
  return string.format("%q", tostring(s))
end

function Typographer:boolean(b)
  return self:_defaultType(b)
end

function Typographer:table(t)
  -- catch circular references
  self:ignore(t)

  local entries = ""
  foreach(t, function(k,v)
    if string.len(entries) > 0 then
      entries = entries .."\n"
    end
    entries = entries .. self(k) .." = ".. self(v)
  end)

  return string.format("{\n%s\n}", entries)
end
  
function Typographer:function_(f)
  local info = debug.getinfo(f, "nu")
  if info ~= nil then
    local params = ""
    for n = 0,info.nups do
      if string.len(params) > 0 then
        params = params ..", "
      end
      params = params .."param".. n
    end
    return string.format("%s(%s)", info.name or "function", params)
  end
end

function Typographer:thread(t)
end

function Typographer:userdata(u)
  local info = class_info(u)
  if info ~= nil then
    local printer = self[info.name]
    if printer ~= nil then
      return printer(self, u)
    elseif u.__tostring then
      return tostring(u)
    else
      return string.format("%s = {\nmethods = %s\nattributes = %s\n}", info.name, self(info.methods), self(info.attributes))
    end
  end
end

Typographer["Vec2"] = function(self, v)
  return string.format("Vec2(%s)", tostring(v))
end

Typographer["Vec3"] = function(self, v)
  return string.format("Vec3(%s)", tostring(v))
end

Typographer["Vec4"] = function(self, v)
  return string.format("Vec4(%s)", tostring(v))
end

Typographer["Rect"] = function(self, r)
  return string.format("Rect(%s)", tostring(r))
end

Typographer["Color"] = function(self, c)
  return string.format("Color(%s)", tostring(c))
end

