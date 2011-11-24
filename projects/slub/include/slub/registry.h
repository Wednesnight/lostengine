#ifndef SLUB_REGISTRY_H
#define SLUB_REGISTRY_H

#include "forward.h"
#include "slub_lua.h"

#include <string>
#include <map>
#include <list>
#include <iostream>
#include <typeinfo>

namespace slub {

  template<typename T>
  struct type_holder {
    static const std::type_info* type;
    static std::string typeName;
  };

  template<typename T>
  const std::type_info* type_holder<T>::type = NULL;

  template<typename T>
  std::string type_holder<T>::typeName;

  struct registry;

  struct registry_holder : public std::map<const std::type_info*, registry*> {
    ~registry_holder();
  };
  
  struct registry {

  public:

    friend class registry_holder;

    template<typename T>
    static registry* registerType(const std::string& typeName) {
      type_holder<T>::type = &typeid(T);
      type_holder<T>::typeName = typeName;
      return get<T>();
    }

    template<typename T>
    static bool isRegisteredType() {
      return type_holder<T>::type != NULL;
    }

    static registry* get(const std::type_info& type) {
      return instance.find(&type) != instance.end() ? instance[&type] : NULL;
    }

    const std::type_info& getType() {
      return type;
    }
    
    std::string getTypeName() {
      return typeName;
    }

    void addConstructor(abstract_constructor* ctor);
    bool containsConstructor();
    abstract_constructor* getConstructor(lua_State* L);
    
    void addField(const std::string& fieldName, abstract_field* field);
    bool containsField(const std::string& fieldName);
    abstract_field* getField(void* v, const std::string& fieldName, bool throw_ = true);
    
    void addMethod(const std::string& methodName, abstract_method* method);
    bool containsMethod(const std::string& methodName);
    abstract_method* getMethod(const std::string& methodName, lua_State* L, bool throw_ = true);
    
    void addOperator(const std::string& operatorName, abstract_operator* op);
    bool containsOperator(const std::string& operatorName);
    abstract_operator* getOperator(const std::string& operatorName, lua_State* L, bool throw_ = true);
    
    void registerBase(registry* base);
    bool hasBase();
    const std::list<registry*> baseList();

  private:

    template<typename T>
    static std::string getTypeName() {
      return type_holder<T>::typeName;
    }

    template<typename T>
    static registry* get() {
      registry* reg = NULL;
      const std::type_info& type = typeid(T);
      std::map<const std::type_info*, registry*>::iterator iter = instance.find(&type);
      if (iter == instance.end()) {
        reg = new registry(type, getTypeName<T>());
        instance[&type] = reg;
      }
      else {
        reg = iter->second;
      }
      return reg;
    }

    static registry_holder instance;

    registry(const std::type_info& type, const std::string& typeName);
    ~registry();
    
    const std::type_info& type;
    std::string typeName;
    
    std::list<abstract_constructor*> constructors;

    std::map<std::string, abstract_field*> fieldMap;
    std::map<std::string, std::list<abstract_method*> > methodMap;
    std::map<std::string, std::list<abstract_operator*> > operatorMap;

    std::list<registry*> baseList_;

  };

}

#endif
