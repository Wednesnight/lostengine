#ifndef LOST_COMMON_FUNCTION_H
#define LOST_COMMON_FUNCTION_H

namespace lost {

  namespace common {

    struct empty_argument{};

    /*
     * function templates
     */
    
    template<typename resultType = void, typename arg1 = empty_argument, typename arg2 = empty_argument>
    struct function {
    private:
      resultType(*func)(arg1, arg2);
    protected:
      function() : func(NULL) {
      }
    public:
      function(resultType(*func)(arg1, arg2)) : func(func) {
      }

      resultType operator()(arg1 a1, arg2 a2) {
        return func(a1, a2);
      }
    };

    template<typename arg1, typename arg2>
    struct function<void, arg1, arg2> {
    private:
      void(*func)(arg1, arg2);
    protected:
      function() : func(NULL) {
      }
    public:
      function(void(*func)(arg1, arg2)) : func(func) {
      }
      
      void operator()(arg1 a1, arg2 a2) {
        func(a1, a2);
      }
    };
    
    template<typename resultType, typename arg1>
    struct function<resultType, arg1, empty_argument> {
    private:
      resultType(*func)(arg1);
    protected:
      function() : func(NULL) {
      }
    public:
      function(resultType(*func)(arg1)) : func(func) {
      }
      
      resultType operator()(arg1 a1) {
        return func(a1);
      }
    };
    
    template<typename arg1>
    struct function<void, arg1, empty_argument> {
    private:
      void(*func)(arg1);
    protected:
      function() : func(NULL) {
      }
    public:
      function(void(*func)(arg1)) : func(func) {
      }
      
      void operator()(arg1 a1) {
        func(a1);
      }
    };
    
    template<typename resultType>
    struct function<resultType, empty_argument, empty_argument> {
    private:
      resultType(*func)();
    protected:
      function() : func(NULL) {
      }
    public:
      function(resultType(*func)()) : func(func) {
      }
      
      resultType operator()() {
        return func();
      }
    };
    
    template<>
    struct function<void, empty_argument, empty_argument> {
    private:
      void(*func)();
    protected:
      function() : func(NULL) {
      }
    public:
      function(void(*func)()) : func(func) {
      }
      
      void operator()() {
        func();
      }
    };

    template<typename resultType, typename arg1, typename arg2>
    function<resultType, arg1, arg2> bind(resultType(*func)(arg1, arg2)) {
      return function<resultType, arg1, arg2>(func);
    }
    
    template<typename arg1, typename arg2>
    function<void, arg1, arg2> bind(void(*func)(arg1, arg2)) {
      return function<void, arg1, arg2>(func);
    }
    
    template<typename resultType, typename arg1>
    function<resultType, arg1, empty_argument> bind(resultType(*func)(arg1)) {
      return function<resultType, arg1, empty_argument>(func);
    }
    
    template<typename arg1>
    function<void, arg1, empty_argument> bind(void(*func)(arg1)) {
      return function<void, arg1, empty_argument>(func);
    }
    
    template<typename resultType>
    function<resultType, empty_argument, empty_argument> bind(resultType(*func)()) {
      return function<resultType, empty_argument, empty_argument>(func);
    }
    
    template<>
    function<void, empty_argument, empty_argument> bind(void(*func)()) {
      return function<void, empty_argument, empty_argument>(func);
    }

    /*
     * member_function templates
     */

    template<typename T, typename resultType = void, typename arg1 = empty_argument, typename arg2 = empty_argument>
    struct member_function : public function<resultType, arg1, arg2> {
    private:
      T* instance;
      resultType(T::*func)(arg1, arg2);
    public:
      member_function(T* instance, resultType(T::*func)(arg1, arg2)) : instance(instance), func(func) {
      }
      
      resultType operator()(arg1 a1, arg2 a2) {
        return instance->func(a1, a2);
      }
    };
    
    template<typename T, typename arg1, typename arg2>
    struct member_function<T, void, arg1, arg2> : public function<void, arg1, arg2> {
    private:
      T* instance;
      void(T::*func)(arg1, arg2);
    public:
      member_function(T* instance, void(T::*func)(arg1, arg2)) : instance(instance), func(func) {
      }
      
      void operator()(arg1 a1, arg2 a2) {
        instance->func(a1, a2);
      }
    };
    
    template<typename T, typename resultType, typename arg1>
    struct member_function<T, resultType, arg1, empty_argument> : public function<resultType, arg1, empty_argument> {
    private:
      T* instance;
      resultType(T::*func)(arg1);
    public:
      member_function(T* instance, resultType(T::*func)(arg1)) : instance(instance), func(func) {
      }
      
      resultType operator()(arg1 a1) {
        return instance->func(a1);
      }
    };
    
    template<typename T, typename arg1>
    struct member_function<T, void, arg1, empty_argument> : public function<void, arg1, empty_argument> {
    private:
      T* instance;
      void(T::*func)(arg1);
    public:
      member_function(T* instance, void(T::*func)(arg1)) : instance(instance), func(func) {
      }
      
      void operator()(arg1 a1) {
        instance->func(a1);
      }
    };
    
    template<typename T, typename resultType>
    struct member_function<T, resultType, empty_argument, empty_argument> : public function<resultType, empty_argument, empty_argument> {
    private:
      T* instance;
      resultType(T::*func)();
    public:
      member_function(T* instance, resultType(T::*func)()) : instance(instance), func(func) {
      }
      
      resultType operator()() {
        return instance->func();
      }
    };
    
    template<typename T>
    struct member_function<T, void, empty_argument, empty_argument> : public function<void, empty_argument, empty_argument> {
    private:
      T* instance;
      void(T::*func)();
    public:
      member_function(T* instance, void(T::*func)()) : instance(instance), func(func) {
      }
      
      void operator()() {
        instance->func();
      }
    };
    
    template<typename T, typename resultType, typename arg1, typename arg2>
    function<resultType, arg1, arg2> bind(T* instance, resultType(T::*func)(arg1, arg2)) {
      return member_function<T, resultType, arg1, arg2>(instance, func);
    }

    template<typename T, typename arg1, typename arg2>
    function<void, arg1, arg2> bind(T* instance, void(T::*func)(arg1, arg2)) {
      return member_function<T, void, arg1, arg2>(instance, func);
    }
    
    template<typename T, typename resultType, typename arg1>
    function<resultType, arg1, empty_argument> bind(T* instance, resultType(T::*func)(arg1)) {
      return member_function<T, resultType, arg1, empty_argument>(instance, func);
    }
    
    template<typename T, typename arg1>
    function<void, arg1, empty_argument> bind(T* instance, void(T::*func)(arg1)) {
      return member_function<T, void, arg1, empty_argument>(instance, func);
    }
    
    template<typename T, typename resultType>
    function<resultType, empty_argument, empty_argument> bind(T* instance, resultType(T::*func)()) {
      return member_function<T, resultType, empty_argument, empty_argument>(instance, func);
    }
    
    template<typename T>
    function<void, empty_argument, empty_argument> bind(T* instance, void(T::*func)()) {
      return member_function<T, void, empty_argument, empty_argument>(instance, func);
    }
    
  }
}

#endif
