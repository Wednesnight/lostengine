
function xpos(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("xpos rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

function xleft(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("xleft rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

function xcenter(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("xcenter rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

function xright(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("xright rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

function ypos(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("ypos rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

function width(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("width rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

function height(t)
  t = t or {}
  local v = {rel = t.rel or 0, abs = t.abs or 0}
  return function()
    print("height rel: "..tostring(v.rel).." abs:"..tostring(v.abs))
  end
end

builtins = {xpos, ypos, width, height, 
            ["xleft"] = xleft, ["xcenter"] = xcenter, ["xright"] = xright}

function isBuiltin(f)
  result = false
  assert(type(f)=="function", "only functions can be builtins")
  for k,v in pairs(builtins) do
    if v == f then 
      result = true
      break
    end
  end
  return result
end

function is4Array(t)
  local result = true
  if not (#t == 4) then result = false end
  for k,v in pairs(t) do
    if not (type(k) == "number") then result = false end
  end
  return result
end

-- index = [1, 4], x,y,w,h
-- default functions for indices
-- 1 = xpos
-- 2 = ypos
-- 3 = width
-- 4 = height

local defaults = {xpos,ypos,width,height}

function decodeEntry(index, entry)
  assert((index>=1 and index <=4), "entry index must be in range [1,4]")
  local result = nil
  
  -- function
  if type(entry) == "function" then
    if isBuiltin(entry) then
      result = entry() -- builtins must be called to build the default functions
    else
      result = entry -- other functions are copied verbatim
    end
  end
  
  -- number
  if type(entry) == "number" then
    -- absolute value, find default function and pass it in 
    result = defaults[index]({abs=entry})
  end

  -- string
  if type(entry) == "string" then
    -- relative value, find default function and pass it in, OR builtin function
    local builtin = builtins[entry]
    if builtin then
      result = builtin()
    else
      result = defaults[index]({rel=entry})
    end
  end
  
  -- table, contains abs and/or rel values, pass it in to default function
  -- if any of the parameters is a bultin function, it will override the default
  local builtin = nil
  if type(entry) == "table" then
    local t = {}
    for k,v in pairs(entry) do
      if type(v) == "string" then
        local bi = builtins[v]
        if not bi then
          t.rel = tonumber(v)
        else
          builtin = bi
        end
      elseif type(v) == "number" then 
        t.abs = v
      end
    end
    if builtin then
      result = builtin(t)
    else
      result = defaults[index](t)
    end
  end
  
  return result
end

function decode(t)
  assert(is4Array(t), "bounds table must be an array with four entries")
  return {decodeEntry(1,t[1]),
          decodeEntry(2,t[2]),
          decodeEntry(3,t[3]),
          decodeEntry(4,t[4])}
end

function test(t)
  print("---------")
  local rt = decode(t)
  for k,v in pairs(rt) do
    v()
  end 
end

test({0,0,50,50})
test({".5",0,50,50})
test({{".5", 10},0,50,50})
test({xleft,0,50,50})
test({"xleft",0,50,50})
test({{"xleft", ".5", 10},0,50,50})
test({{"xcenter", ".75"},0,50,50})
test({{"xright", 10},0,50,50})


print("ok")
