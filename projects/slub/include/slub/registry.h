#ifndef SLUB_REGISTRY_H
#define SLUB_REGISTRY_H

#include "config.h"
#include "forward.h"
#include "slub_lua.h"

#include <map>
#include <list>
#include <iostream>
#include <typeinfo>

namespace slub {

  template<typename T>
  struct type_holder {
    static const std::type_info* type;
    static string typeName;
  };

  template<typename T>
  const std::type_info* type_holder<T>::type = NULL;

  template<typename T>
  string type_holder<T>::typeName;

  struct registry;

  struct registry_holder : public std::map<const std::type_info*, registry*> {
    ~registry_holder();
  };
  
  struct registry {

  public:

    friend class registry_holder;

    template<typename T>
    static registry* registerType(const string& typeName) {
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
    
    string getTypeName() {
      return typeName;
    }

    void addConstructor(abstract_constructor* ctor);
    bool containsConstructor();
    abstract_constructor* getConstructor(lua_State* L);
    
    void addField(const string& fieldName, abstract_field* field);
    bool containsField(const string& fieldName);
    abstract_field* getField(void* v, const string& fieldName, bool throw_ = true);
    
    void addMethod(const string& methodName, abstract_method* method);
    bool containsMethod(const string& methodName);
    abstract_method* getMethod(const string& methodName, lua_State* L, bool throw_ = true);
    
    void addOperator(const string& operatorName, abstract_operator* op);
    bool containsOperator(const string& operatorName);
    abstract_operator* getOperator(const string& operatorName, lua_State* L, bool throw_ = true);
    
    void registerBase(registry* base);
    bool hasBase();
    const std::list<registry*> baseList();

  private:

    template<typename T>
    static string getTypeName() {
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

    registry(const std::type_info& type, const string& typeName);
    ~registry();
    
    const std::type_info& type;
    string typeName;
    
    std::list<abstract_constructor*> constructors;

    std::map<string, abstract_field*> fieldMap;
    std::map<string, std::list<abstract_method*> > methodMap;
    std::map<string, std::list<abstract_operator*> > operatorMap;

    std::list<registry*> baseList_;

  };

}

#endif
