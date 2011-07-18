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
    std::cout << className +": cleanup constructors" << std::endl;
    for (std::list<abstract_constructor*>::iterator midx = constructors.begin(); midx != constructors.end(); ++midx) {
      delete *midx;
    }
    constructors.clear();
    
    std::cout << className +": cleanup fields" << std::endl;
    for (std::map<std::string, abstract_field*>::iterator idx = fieldMap.begin(); idx != fieldMap.end(); ++idx) {
      delete idx->second;
    }
    fieldMap.clear();
    
    std::cout << className +": cleanup methods" << std::endl;
    for (std::map<std::string, std::list<abstract_method*> >::iterator idx = methodMap.begin(); idx != methodMap.end(); ++idx) {
      for (std::list<abstract_method*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    methodMap.clear();
    
    std::cout << className +": cleanup operators" << std::endl;
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
    return fieldMap.find(fieldName) != fieldMap.end();
  }
  
  abstract_field* registry::getField(const std::string& fieldName) {
    if (!containsField(fieldName)) {
      throw FieldNotFoundException(fieldName);
    }
    
    return fieldMap[fieldName];
  }
  
  void registry::addMethod(const std::string& methodName, abstract_method* method) {
    methodMap[methodName].push_back(method);
  }
  
  bool registry::containsMethod(const std::string& methodName) {
    return methodMap.find(methodName) != methodMap.end();
  }
  
  abstract_method* registry::getMethod(const std::string& methodName, lua_State* L) {
    if (!containsMethod(methodName)) {
      throw MethodNotFoundException(methodName);
    }
    
    for (std::list<abstract_method*>::iterator midx = methodMap[methodName].begin(); midx != methodMap[methodName].end(); ++midx) {
      if ((*midx)->check(L)) {
        return *midx;
      }
    }
    throw OverloadNotFoundException(methodName);
  }
  
  void registry::addOperator(const std::string& operatorName, abstract_operator* op) {
    operatorMap[operatorName].push_back(op);
  }
  
  bool registry::containsOperator(const std::string& operatorName) {
    return operatorMap.find(operatorName) != operatorMap.end();
  }
  
  abstract_operator* registry::getOperator(const std::string& operatorName, lua_State* L) {
    if (!containsOperator(operatorName)) {
      throw OperatorNotFoundException(operatorName);
    }
    
    for (std::list<abstract_operator*>::iterator oidx = operatorMap[operatorName].begin(); oidx != operatorMap[operatorName].end(); ++oidx) {
      if ((*oidx)->check(L)) {
        return *oidx;
      }
    }
    throw OverloadNotFoundException(operatorName);
  }

  registry* registry::get(const std::string& className) {
    if (instance.find(className) == instance.end()) {
      instance[className] = new registry(className);
    }
    return instance[className];
  }
  
}
