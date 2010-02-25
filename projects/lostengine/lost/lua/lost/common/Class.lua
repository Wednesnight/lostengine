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
      if not ancestor.bases then
        error("can't find class hierarchy for '"..class.."', you might have forgotten a require or misspelled the class name.",3)
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
    Expects a string that contains a dot delimited fully qualified class name.
    Splits the string at the dots and creates tables for each namespace level, 
    placing the result in the global namespace _G.
    Example:
    'lost.guiro.View' would be split into 'lost', 'guiro', 'View'
    Tables would be created to enable the following expression:
    _G["lost"]["guiro"]["View"]
    Initially, the result of this expression is an empty table.
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

  -- checks if self is an instance or is derived from fullyQualifiedClassname
  -- the function iterates over all fully qualified base class names and compares
  -- them against provided string
  namespace.isDerivedFrom = function (self, fullyQualifiedClassname)
    local result = self.bases or false
    if result then
      for k,base in next,self.bases do
        result = (base == fullyQualifiedClassname)
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
  
  -- __tostring operator
  if namespace.__tostring == nil then
    namespace.__tostring = function(self)
      return self:className()
    end
  end

  -- __eq (equal) operator
  if namespace.__eq == nil then
    namespace.__eq = function(self, other)
      return rawequal(self, other)
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

      -- call the ctor
      if type(object.constructor) == "function" then
        object:constructor(unpack(arg))
      end

      -- return the new instance
      return object
    end
  }

  -- set the class ctor
  setmetatable(namespace, metatable)
end
