#include "../../include/slub/config.h"
#include "../../include/slub/registry.h"
#include "../../include/slub/constructor.h"
#include "../../include/slub/exception.h"
#include "../../include/slub/field.h"
#include "../../include/slub/method.h"
#include "../../include/slub/operators.h"

#include <iostream>
#include <stdexcept>

namespace slub {

  registry_holder registry::instance;

  registry_holder::~registry_holder() {
    for (std::map<const std::type_info*, registry*>::iterator idx = begin(); idx != end(); ++idx) {
      delete idx->second;
    }
    clear();
  }

  registry::registry(const std::type_info& type, const string& typeName)
  : type(type), typeName(typeName)
  {
  }

  registry::~registry() {
    for (std::list<abstract_constructor*>::iterator midx = constructors.begin(); midx != constructors.end(); ++midx) {
      delete *midx;
    }
    constructors.clear();
    
    for (std::map<string, abstract_field*>::iterator idx = fieldMap.begin(); idx != fieldMap.end(); ++idx) {
      delete idx->second;
    }
    fieldMap.clear();
    
    for (std::map<string, std::list<abstract_method*> >::iterator idx = methodMap.begin(); idx != methodMap.end(); ++idx) {
      for (std::list<abstract_method*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    methodMap.clear();
    
    for (std::map<string, std::list<abstract_operator*> >::iterator idx = operatorMap.begin(); idx != operatorMap.end(); ++idx) {
      for (std::list<abstract_operator*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    operatorMap.clear();
  }
  
  void registry::addConstructor(abstract_constructor* ctor) {
    constructors.push_back(ctor);
  }
  
  bool registry::containsConstructor() {
    return constructors.size() > 0;
  }
  
  abstract_constructor* registry::getConstructor(lua_State* L) {
    if (!containsConstructor()) {
      throw ConstructorNotFoundException(typeName);
    }
    
    for (std::list<abstract_constructor*>::iterator cidx = constructors.begin(); cidx != constructors.end(); ++cidx) {
      if ((*cidx)->check(L)) {
        return *cidx;
      }
    }
    throw OverloadNotFoundException(typeName);
  }
  
  void registry::addField(const string& fieldName, abstract_field* field) {
    fieldMap[fieldName] = field;
  }
  
  bool registry::containsField(const string& fieldName) {
    bool result = fieldMap.find(fieldName) != fieldMap.end();
    if (!result && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); !result && idx != baseList_.end(); ++idx) {
        result = (*idx)->containsField(fieldName);
      }
    }
    return result;
  }
  
  abstract_field* registry::getField(void* v, const string& fieldName, bool throw_) {
    abstract_field* result = NULL;
    if (fieldMap.find(fieldName) != fieldMap.end()) {
      result = fieldMap[fieldName];
    }
    if (result == NULL && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); result == NULL && idx != baseList_.end(); ++idx) {
        result = (*idx)->getField(v, fieldName, false);
      }
    }

    if (result == NULL && throw_) {
      throw FieldNotFoundException(typeName +"."+ fieldName);
    }
    return result;
  }
  
  void registry::addMethod(const string& methodName, abstract_method* method) {
    methodMap[methodName].push_back(method);
  }
  
  bool registry::containsMethod(const string& methodName) {
    bool result = methodMap.find(methodName) != methodMap.end();
    if (!result && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); !result && idx != baseList_.end(); ++idx) {
        result = (*idx)->containsMethod(methodName);
      }
    }
    return result;
  }
  
  abstract_method* registry::getMethod(const string& methodName, lua_State* L, bool throw_) {
    struct method_not_found : public abstract_method {
      method_not_found() {}
      bool check(lua_State*) {return false;}
      int call(lua_State*) {return 0;}
    };
    static method_not_found not_found;

    abstract_method* result = &not_found;
    if (methodMap.find(methodName) != methodMap.end()) {
      result = NULL;
      for (std::list<abstract_method*>::iterator midx = methodMap[methodName].begin();
           (result == &not_found || result == NULL) && midx != methodMap[methodName].end(); ++midx) {
        if ((*midx)->check(L)) {
          result = *midx;
        }
      }
    }
    if ((result == &not_found || result == NULL) && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin();
           (result == &not_found || result == NULL) && idx != baseList_.end(); ++idx) {
        abstract_method* r = (*idx)->getMethod(methodName, L, false);
        if ((r != &not_found && r != NULL) || result == &not_found) {
          result = r;
        }
      }
    }
    
    if ((result == &not_found || result == NULL) && throw_) {
      string s;
      int n = lua_gettop(L);
      for (int idx = 1; idx < n; ++idx) {
        if (s.size() == 0) {
          s = "(";
        }
        else {
          s += ", ";
        }
        s += luaL_typename(L, -(n-idx));
      }
      if (s.size() > 0) {
        s += ")";
      }

      if (result == &not_found) {
        throw MethodNotFoundException(typeName +"."+ methodName + s);
      }
      else {
        throw OverloadNotFoundException(typeName +"."+ methodName + s);
      }
    }
    return result;
  }
  
  void registry::addOperator(const string& operatorName, abstract_operator* op) {
    operatorMap[operatorName].push_back(op);
  }
  
  bool registry::containsOperator(const string& operatorName) {
    bool result = operatorMap.find(operatorName) != operatorMap.end();
    if (!result && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); !result && idx != baseList_.end(); ++idx) {
        result = (*idx)->containsOperator(operatorName);
      }
    }
    return result;
  }
  
  abstract_operator* registry::getOperator(const string& operatorName, lua_State* L, bool throw_) {
    struct operator_not_found : public abstract_operator {
      operator_not_found() {}
      bool check(lua_State*) {return false;}
      int op(lua_State*) {return 0;}
    };
    static operator_not_found not_found;

    abstract_operator* result = &not_found;
    if (operatorMap.find(operatorName) != operatorMap.end()) {
      result = NULL;
      for (std::list<abstract_operator*>::iterator oidx = operatorMap[operatorName].begin();
           (result == &not_found || result == NULL) && oidx != operatorMap[operatorName].end(); ++oidx)
      {
        if ((*oidx)->check(L)) {
          result = *oidx;
        }
      }
    }
    if ((result == &not_found || result == NULL) && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin();
           (result == &not_found || result == NULL) && idx != baseList_.end(); ++idx)
      {
        abstract_operator* r = (*idx)->getOperator(operatorName, L, false);
        if ((r != &not_found && r != NULL) || result == &not_found) {
          result = r;
        }
      }
    }

    if ((result == &not_found || result == NULL) && throw_) {
      if (result == &not_found) {
        throw OperatorNotFoundException(typeName +"."+ operatorName);
      }
      else {
        throw OverloadNotFoundException(typeName +"."+ operatorName);
      }
    }
    return result;
  }

  void registry::registerBase(registry* base) {
    baseList_.push_back(base);
  }

  bool registry::hasBase() {
    return baseList_.size() > 0;
  }

  const std::list<registry*> registry::baseList() {
    return baseList_;
  }

}
