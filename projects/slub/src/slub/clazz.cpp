#include <slub/clazz.h>

#include <iostream>
#include <stdexcept>

namespace slub {

  fields fields::instance;
  
  fields::~fields() {
    std::cout << "cleanup constructors" << std::endl;
    for (std::map<std::string, std::list<abstract_constructor*> >::iterator idx = constructorMap.begin(); idx != constructorMap.end(); ++idx) {
      for (std::list<abstract_constructor*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    constructorMap.clear();
    
    std::cout << "cleanup fields" << std::endl;
    for (std::map<std::string, abstract_field*>::iterator idx = fieldMap.begin(); idx != fieldMap.end(); ++idx) {
      delete idx->second;
    }
    fieldMap.clear();
    
    std::cout << "cleanup methods" << std::endl;
    for (std::map<std::string, std::list<abstract_method*> >::iterator idx = methodMap.begin(); idx != methodMap.end(); ++idx) {
      for (std::list<abstract_method*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    methodMap.clear();
    
    std::cout << "cleanup operators" << std::endl;
    for (std::map<std::string, std::list<abstract_operator*> >::iterator idx = operatorMap.begin(); idx != operatorMap.end(); ++idx) {
      for (std::list<abstract_operator*>::iterator midx = idx->second.begin(); midx != idx->second.end(); ++midx) {
        delete *midx;
      }
    }
    operatorMap.clear();
  }
  
  void fields::addConstructor(const std::string& className, abstract_constructor* ctor) {
    instance.constructorMap[className].push_back(ctor);
  }
  
  bool fields::containsConstructor(const std::string& className) {
    return instance.constructorMap.find(className) != instance.constructorMap.end();
  }
  
  abstract_constructor* fields::getConstructor(const std::string& className, lua_State* L) {
    if (!containsConstructor(className)) {
      throw std::runtime_error("No constructor defined for class: "+ className);
    }
    
    for (std::list<abstract_constructor*>::iterator cidx = instance.constructorMap[className].begin(); cidx != instance.constructorMap[className].end(); ++cidx) {
      if ((*cidx)->check(L)) {
        return *cidx;
      }
    }
    throw std::runtime_error("No matching overload found, candidates: ...");
  }
  
  void fields::addField(const std::string& fieldName, abstract_field* field) {
    instance.fieldMap[fieldName] = field;
  }
  
  bool fields::containsField(const std::string& fieldName) {
    return instance.fieldMap.find(fieldName) != instance.fieldMap.end();
  }
  
  abstract_field* fields::getField(const std::string& fieldName) {
    if (!containsField(fieldName)) {
      throw std::runtime_error("No such field: "+ fieldName);
    }
    
    return instance.fieldMap[fieldName];
  }
  
  void fields::addMethod(const std::string& methodName, abstract_method* method) {
    instance.methodMap[methodName].push_back(method);
  }
  
  bool fields::containsMethod(const std::string& methodName) {
    return instance.methodMap.find(methodName) != instance.methodMap.end();
  }
  
  abstract_method* fields::getMethod(const std::string& methodName, lua_State* L) {
    if (!containsMethod(methodName)) {
      throw std::runtime_error("No such method: "+ methodName);
    }
    
    for (std::list<abstract_method*>::iterator midx = instance.methodMap[methodName].begin(); midx != instance.methodMap[methodName].end(); ++midx) {
      if ((*midx)->check(L)) {
        return *midx;
      }
    }
    throw std::runtime_error("No matching overload found, candidates: ...");
  }
  
  void fields::addOperator(const std::string& operatorName, abstract_operator* op) {
    instance.operatorMap[operatorName].push_back(op);
  }
  
  bool fields::containsOperator(const std::string& operatorName) {
    return instance.operatorMap.find(operatorName) != instance.operatorMap.end();
  }
  
  abstract_operator* fields::getOperator(const std::string& operatorName, lua_State* L) {
    if (!containsOperator(operatorName)) {
      throw std::runtime_error("No such operator: "+ operatorName);
    }
    
    for (std::list<abstract_operator*>::iterator oidx = instance.operatorMap[operatorName].begin(); oidx != instance.operatorMap[operatorName].end(); ++oidx) {
      if ((*oidx)->check(L)) {
        return *oidx;
      }
    }
    throw std::runtime_error("No matching overload found, candidates: ...");
  }
  
}
