module("lost.gl", package.seeall)

require "lost/StringAdditions"

local trim = lost.string.trim
local split = lost.string.split
local normaliseLinebreaks = lost.string.normaliseLinebreaks

local function loadData(loader, path)
--  print("!!! loading: '"..tostring(path).."'")
  local d = loader:load(path):str()
--  print("!!! loaded: "..tostring(d))
  return d
end

local function isDirective(line, name)
  local trimmed = trim(line)
  local sp, ep = string.find(trimmed, "#"..name)
  local result = false
  if sp and ep then 
    result = true
  end
  return result
end

local function isImportDirective(line)
  return isDirective(line, "import")
end

local function isIncludeDirective(line)
  return isDirective(line, "include")
end

local function extractIncludeFilename(line)
  local trimmed = trim(line)
  local splitline = split(trimmed, '"')
  local filename = splitline[2]
  return filename
end

local function preprocessOnce(loader, data, imports)
--  print("============================= ITERATION")
  local result = {}
  data = normaliseLinebreaks(data)
  local splitSource = split(data, "\n")
  local addedParts = 0
  for k,line in pairs(splitSource) do
    if isIncludeDirective(line) then
      local filename = extractIncludeFilename(line)
--      print("++ "..tostring(k).." "..tostring(line).." '"..filename.."'")
      local includeData = loadData(loader,filename)
      table.insert(result, includeData)
      addedParts = addedParts +1 
    elseif isImportDirective(line) then
      local filename  = extractIncludeFilename(line)
      if not imports[filename] then
--        print("adding import "..filename)
        local includeData = loadData(loader,filename)
        table.insert(result, includeData)
        imports[filename] = true
      else
--        print("ignoring import: "..filename)
      end
      addedParts = addedParts +1 
    else
--      print(tostring(k).." "..tostring(line))
      table.insert(result,line)
    end
  end
  return table.concat(result, "\n"),addedParts
end

function preprocessShader(loader, data)
  local added = 1
  local imports = {}
  while added > 0 do
    data,added = preprocessOnce(loader, data, imports)
  end
  return data
end
