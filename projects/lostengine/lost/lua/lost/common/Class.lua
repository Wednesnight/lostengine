module("lost.common", package.seeall)


Class = function(fullyQualifiedName, baseClass)
  if baseClass then
    log.debug("deriving class "..fullyQualifiedName)
    class(fullyQualifiedName)(baseClass)
    local newClass = _G[fullyQualifiedName]
    baseClass:addBase(newClass, fullyQualifiedName)
    return newClass
  else
    log.debug("creating class "..fullyQualifiedName)
    class(fullyQualifiedName)
    local newClass = _G[fullyQualifiedName]
    return newClass
  end
end
