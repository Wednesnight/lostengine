#ifndef SLUB_REGISTRY_H
#define SLUB_REGISTRY_H

#include <slub/forward.h>
#include <slub/slub_lua.h>
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <typeinfo>

namespace slub {

  template<typename T>
  struct type_holder {
    static std::string typeName;
  };

  template<typename T>
  std::string type_holder<T>::typeName;

  struct registry;

  struct registry_holder : public std::map<std::string, registry*> {
    ~registry_holder();
  };
  
  struct registry {

  public:

    friend class registry_holder;

    template<typename T>
    static registry* registerType(const std::string& typeName) {
      type_holder<T>::typeName = typeName;
      return get<T>();
    }

    template<typename T>
    static bool isRegisteredType() {
      return type_holder<T>::typeName.size() > 0;
    }

    template<typename T>
    static std::string getTypeName() {
      return type_holder<T>::typeName;
    }

    template<typename T>
    static registry* get() {
      std::string className = getTypeName<T>();
      if (instance.find(className) == instance.end()) {
        registry* reg = new registry(className);
        instance[className] = reg;
      }
      return instance[className];
    }
    
    std::string getTypeName() {
      return className;
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
    void registerDerived(registry* derived);
    bool isBase();
    const std::list<registry*> derivedList();

  private:

    static registry_holder instance;

    registry(const std::string& className);
    ~registry();
    
    std::string className;
    
    std::list<abstract_constructor*> constructors;

    std::map<std::string, abstract_field*> fieldMap;
    std::map<std::string, std::list<abstract_method*> > methodMap;
    std::map<std::string, std::list<abstract_operator*> > operatorMap;

    std::list<registry*> baseList_;
    std::list<registry*> derivedList_;

  };

}

#endif
