#ifndef _TINYTHREAD_THREAD_SPECIFIC_PTR_H_
#define _TINYTHREAD_THREAD_SPECIFIC_PTR_H_

#include "tinythread.h"

namespace tthread
{

  typedef void(*CleanupFunc)(void*);

  template<typename T>
  struct thread_specific_ptr
  {
  private:

    struct thread_specific_value {
      T* value;
      CleanupFunc cleanupFunc;

      thread_specific_value(T* value, CleanupFunc cleanupFunc = NULL)
      : value(value),
        cleanupFunc(cleanupFunc)
      {
      }

      ~thread_specific_value() {
        if (cleanupFunc != NULL) {
          cleanupFunc(value);
        }
        else {
          delete value;
        }
      }
    };

#if defined(_TTHREAD_WIN32_)
    DWORD _key;
#else
    pthread_key_t _key;
    static void destructor(void* p) {
      thread_specific_value* value = static_cast<thread_specific_value*>(p);
      if (value != NULL) {
        delete value;
      }
    }
#endif  
    CleanupFunc _cleanup;
    
    thread_specific_value* getValue() const {
      thread_specific_value* result;
#if defined(_TTHREAD_WIN32_)
      LPVOID tmp = TlsGetValue(_key);
      result = static_cast<thread_specific_value*>(tmp);
#else
      void* tmp = pthread_getspecific(_key);
      result = static_cast<thread_specific_value*>(tmp);
#endif
      return result;
    }

    void destroy()
    {
      thread_specific_value* ptr = getValue();
      if(ptr) {
        delete ptr;
      }    
    }
    
  public:

    thread_specific_ptr(CleanupFunc cleanupFunc = NULL)
    {
#if defined(_TTHREAD_WIN32_)
      _key = TlsAlloc(); // FIXME how should errors be handled?
#else
      _key = pthread_key_t();
      pthread_key_create(&_key, destructor); // FIXME how should errors be handled?
#endif    
      _cleanup = cleanupFunc;
    }
    
    ~thread_specific_ptr()
    {
      destroy();
#if defined(_TTHREAD_WIN32_)
      TlsFree(_key);
#else
      pthread_key_delete(_key);
#endif
    }
    
    T* get() const
    {
      thread_specific_value* result = getValue();
      return (result != NULL ? result->value : NULL);
    }
    
    T* operator->() const
    {
        return get();
    }
    
    T& operator*() const
    {
        return *get();
    }
    
    void reset(T* new_value=0)
    {
        T* const current_value = get();
        if(current_value != new_value) {
          destroy();
#if defined(_TTHREAD_WIN32_)
          if (!TlsSetValue(_key, (LPVOID) new thread_specific_value(new_value, _cleanup))) {
            printf("set specific failed with error: %d", GetLastError());
          }
#else
          int err = pthread_setspecific(_key, (void *) new thread_specific_value(new_value, _cleanup));
          if(err) {
            printf("set specific failed with error: %d",err);
          }
#endif
        }
    }
    
  };
  
}

#endif
