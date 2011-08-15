#include <slub/registry.h>
#include <slub/constructor.h>
#include <slub/exception.h>
#include <slub/field.h>
#include <slub/method.h>
#include <slub/operators.h>

#include <iostream>
#include <stdexcept>

namespace slub {

  registry_holder registry::instance;

  registry_holder::~registry_holder() {
    for (std::map<std::string, registry*>::iterator idx = begin(); idx != end(); ++idx) {
      delete idx->second;
    }
    clear();
  }

  registry::registry(const std::string& className) : className(className) {
  }

  registry::~registry() {
//    std::cout << className +": cleanup constructors" << std::endl;
    for (std::list<abstract_constructor*>::iterator midx = constructors.begin(); midx != constructors.end(); ++midx) {
      delete *midx;
    }
    constructors.clear();
    
//    std::cout << className +": cleanup fields" << std::endl;
    for (std::map<std::string, abstract_field*>::iterator idx = fieldMap.begin(); idx != fieldMap.end(); ++idx) {
      delete idx->second;
    }
    fieldMap.clear();
    
//    std::cout << className +": cleanup methods" << std::endl;
    for (std::map<std::string, std::list<abstract_method*> >::iterator idx = methodMap.begin(); idx != methodMap.end(); ++idx) {
      for (std::list<abstract_method*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    methodMap.clear();
    
//    std::cout << className +": cleanup operators" << std::endl;
    for (std::map<std::string, std::list<abstract_operator*> >::iterator idx = operatorMap.begin(); idx != operatorMap.end(); ++idx) {
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
      throw ConstructorNotFoundException(className);
    }
    
    for (std::list<abstract_constructor*>::iterator cidx = constructors.begin(); cidx != constructors.end(); ++cidx) {
      if ((*cidx)->check(L)) {
        return *cidx;
      }
    }
    throw OverloadNotFoundException(className);
  }
  
  void registry::addField(const std::string& fieldName, abstract_field* field) {
    fieldMap[fieldName] = field;
  }
  
  bool registry::containsField(const std::string& fieldName) {
    bool result = fieldMap.find(fieldName) != fieldMap.end();
    if (!result && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); !result && idx != baseList_.end(); ++idx) {
        result = (*idx)->containsField(fieldName);
      }
    }
    return result;
  }
  
  abstract_field* registry::getField(void* v, const std::string& fieldName, bool throw_) {
    if (!containsField(fieldName) && throw_) {
      throw FieldNotFoundException(fieldName);
    }

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
      throw OverloadNotFoundException(className +"."+ fieldName);
    }
    return result;
  }
  
  void registry::addMethod(const std::string& methodName, abstract_method* method) {
    methodMap[methodName].push_back(method);
  }
  
  bool registry::containsMethod(const std::string& methodName) {
    bool result = methodMap.find(methodName) != methodMap.end();
    if (!result && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); !result && idx != baseList_.end(); ++idx) {
        result = (*idx)->containsMethod(methodName);
      }
    }
    return result;
  }
  
  abstract_method* registry::getMethod(const std::string& methodName, lua_State* L, bool throw_) {
    if (!containsMethod(methodName) && throw_) {
      throw MethodNotFoundException(methodName);
    }
    
    abstract_method* result = NULL;
    if (methodMap.find(methodName) != methodMap.end()) {
      for (std::list<abstract_method*>::iterator midx = methodMap[methodName].begin(); midx != methodMap[methodName].end(); ++midx) {
        if ((*midx)->check(L)) {
          result = *midx;
        }
      }
    }
    if (result == NULL && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); result == NULL && idx != baseList_.end(); ++idx) {
        result = (*idx)->getMethod(methodName, L, false);
      }
    }
    
    if (result == NULL && throw_) {
      std::string s;
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
      throw OverloadNotFoundException(className +"."+ methodName + s);
    }
    return result;
  }
  
  void registry::addOperator(const std::string& operatorName, abstract_operator* op) {
    operatorMap[operatorName].push_back(op);
  }
  
  bool registry::containsOperator(const std::string& operatorName) {
    bool result = operatorMap.find(operatorName) != operatorMap.end();
    if (!result && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); !result && idx != baseList_.end(); ++idx) {
        result = (*idx)->containsOperator(operatorName);
      }
    }
    return result;
  }
  
  abstract_operator* registry::getOperator(const std::string& operatorName, lua_State* L, bool throw_) {
    if (!containsOperator(operatorName) && throw_) {
      throw OperatorNotFoundException(operatorName);
    }
    
    abstract_operator* result = NULL;
    if (operatorMap.find(operatorName) != operatorMap.end()) {
      for (std::list<abstract_operator*>::iterator oidx = operatorMap[operatorName].begin(); oidx != operatorMap[operatorName].end(); ++oidx) {
        if ((*oidx)->check(L)) {
          result = *oidx;
        }
      }
    }
    if (result == NULL && hasBase()) {
      for (std::list<registry*>::iterator idx = baseList_.begin(); result == NULL && idx != baseList_.end(); ++idx) {
        result = (*idx)->getOperator(operatorName, L, false);
      }
    }
    
    if (result == NULL && throw_) {
      throw OverloadNotFoundException(className +"."+ operatorName);
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

  void registry::registerDerived(registry* derived) {
    derivedList_.push_back(derived);
  }

  bool registry::isBase() {
    return derivedList_.size() > 0;
  }

  const std::list<registry*> registry::derivedList() {
    return derivedList_;
  }

}
