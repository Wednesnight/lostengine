module("lost.common", package.seeall)

--[[
    triggers expandNamespace, declareClass to fill the class namespace and set the given definition
    checks by param type: 
      string: first param defines class name, following params define base classes (multiple inheritence possible) -> expandNamespace
      table: class definition -> declareClass
  ]]
function Class(class)
  local namespace = expandNamespace(class)
  local bases = {class}
  local definition = {}
  _class_derived = function(class)
    if type(class) == "string" then
      local ancestor = expandNamespace(class)
      for k,v in next,ancestor do
        definition[k] = v
      end
      for k,v in next,ancestor.bases do
        table.insert(bases, 1, v)
      end
      return _class_derived
    elseif type(class) == "table" then
      for k,v in next,class do
        definition[k] = v
      end
      return declareClass(namespace, bases, definition)
    else
      log.error("_class: invalid param of type ".. type(class))
    end
  end
  return _class_derived
end

--[[
    expands the given namespace
    initializes an empty table (if not existent) for each name in className, seperated by "."
  ]]
function expandNamespace(className)
  local namespace = _G
  for name in string.gmatch(className, "[^%.]+") do
    if not namespace[name] then
      namespace[name] = {}
    end
    namespace = namespace[name]
  end
  return namespace
end

--[[
    fills the given namespace with all members of definition
    fills the class metatable: ctor, dtor listener
  ]]
function declareClass(namespace, bases, definition)

  -- init the class definition
  for k,v in next,definition do
    namespace[k] = v
  end
  namespace.bases = bases

  -- checks if self is a or derived from className
  namespace.is = function (self, className)
    local result = self.bases or false
    if result then
      for k,base in next,self.bases do
        result = base == className
        if result then
          break
        end
      end
    end
    return result
  end

  -- returns the last entry in self.bases
  namespace.className = function(self)
    local result = "nil"
    if self.bases then
      result = self.bases[table.maxn(self.bases)]
    end
    return result
  end

  -- prints self.bases hierarchy
  namespace.printBases = function(self)
    local prefix = ""
    for k,v in next,self.bases do
      log.debug(prefix .."|-- ".. v)
      prefix = prefix .."    "
    end
  end

  
  -- init class metatable
  local metatable =
  {
    __call = function(self, ...)

      -- init new instance
      local object = {}

      -- assign members, methods
      for key,value in next,self do
        object[key] = value
      end

      -- assign operators
      setmetatable(object, self)

      -- make sure that free is our cleanup function
      object.free = function(self)
        self = nil
      end

      -- call the ctor
      if type(object.create) == "function" then
        object:create(unpack(arg))
      end

      -- return the new instance
      return object
    end
  }

  -- set the class ctor
  setmetatable(namespace, metatable)
end
