module("lost.common", package.seeall)

function foreach(table, callback)
  if type(table) ~= "table" then
    log.error(string.format("Invalid argument supplied for foreach (table), expected \"table\", got \"%s\"", type(table)))
    return false
  end
  if type(callback) ~= "function" then
    log.error(string.format("Invalid argument supplied for foreach (callback), expected \"function\", got \"%s\"", type(callback)))
    return false
  end

  for k,v in next,table do
    if callback(k,v) == true then
      return true
    end
  end
  return false
end
