/*
 Copyright (c) 2011 Tony Kostanjsek, Timo Boll
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
 following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LOST_COMMON_FUNCTION_H
#define LOST_COMMON_FUNCTION_H

namespace lost {
  namespace common {

    template<typename Signature> struct FunctionBase {};
    
    template<typename R>
    struct FunctionBase<R(*)()> {
      virtual R operator()() = 0;
      virtual bool operator!() = 0;
    };
    
    template<typename R, typename T1>
    struct FunctionBase<R(*)(T1)> {
      virtual R operator()(T1) = 0;
    };
    
    template<typename Signature> struct Function {};
    
    template<typename R>
    struct Function<R(*)()> : public FunctionBase<R(*)()> {
      
    public:
      typedef R(*Signature)();
      typedef R ResultType;
      
    private:
      Signature s;
      shared_ptr<FunctionBase<Signature> > target;
      
    public:
      Function(FunctionBase<Signature> *target, Signature s)
      : s(NULL),
        target(target)
      {
      }
      
      Function(Signature s)
      : s(s)
      {
      }
      
      ResultType operator()() {
        return s != NULL ? (*s)() : target->operator()();
      }
      
      bool operator!() {
        return s != NULL || target != NULL;
      }

    };
    
    template<typename R, typename T1>
    struct Function<R(*)(T1)> : public FunctionBase<R(*)(T1)> {
      
    public:
      typedef R(*Signature)(T1);
      typedef R ResultType;
      
    private:
      Signature s;
      shared_ptr<FunctionBase<Signature> > target;
      
    public:
      Function(FunctionBase<Signature> *target, Signature s)
      : s(NULL),
        target(target)
      {
      }
      
      Function(Signature s)
      : s(s)
      {
      }
      
      ResultType operator()(T1 t1) {
        return s != NULL ? (*s)(t1) : target->operator()(t1);
      }
      
      bool operator!() {
        return s != NULL || target != NULL;
      }
      
    };
    
    template<typename T, typename R>
    struct Function<R(T::*)()> : public FunctionBase<R(*)()> {
      
    public:
      typedef R(T::*Signature)();
      typedef R ResultType;
      
    private:
      Signature s;
      T* t;
      
    public:
      Function(Signature s, T* t)
      : s(s),
        t(t)
      {
      }
      
      ResultType operator()() {
        return (t->*s)();
      }
      
      bool operator!() {
        return t != NULL && s != NULL;
      }
      
      static Function<R(*)()> bind(Signature s, T* t) {
        return Function<R(*)()>(new Function<Signature>(s, t), NULL);
      }
      
    };
    
    template<typename T, typename R, typename T1>
    struct Function<R(T::*)(T1)> : public FunctionBase<R(*)(T1)> {
      
    public:
      typedef R(T::*Signature)(T1);
      typedef R ResultType;
      
    private:
      Signature s;
      T* t;
      
    public:
      Function(Signature s, T* t)
      : s(s),
        t(t)
      {
      }
      
      ResultType operator()(T1 t1) {
        return (t->*s)(t1);
      }
      
      bool operator!() {
        return t != NULL && s != NULL;
      }
      
    };

    template<typename R, typename T>
    static Function<R(*)()> bind(R(T::*s)(), T* t) {
      return Function<R(*)()>(new Function<R(T::*)()>(s, t), NULL);
    }

    template<typename R, typename T1, typename T>
    static Function<R(*)(T1)> bind(R(T::*s)(T1), T* t) {
      return Function<R(*)(T1)>(new Function<R(T::*)(T1)>(s, t), NULL);
    }

  }
}

#endif
