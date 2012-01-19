#ifndef _TINYTHREAD_THREAD_SPECIFIC_PTR_H_
#define _TINYTHREAD_THREAD_SPECIFIC_PTR_H_

#include "tinythread.h"

namespace tinythread
{
  
template<typename T>
struct thread_specific_ptr
{
#if defined(_TTHREAD_WIN32_)
  DWORD _key;
#else
  pthread_key_t _key;
#endif  
  
  thread_specific_ptr()
  {
#if defined(_TTHREAD_WIN32_)
    _key = TlsAlloc(); // FIXME how should errors be handled?
#else
    _key = NULL;
    pthread_key_create(&_key, NULL); // FIXME how should errors be handled?
#endif    
  }
  
  ~thread_specific_ptr()
  {
    T* ptr = get();
    if(ptr)
    {
      delete ptr;
    }
#if defined(_TTHREAD_WIN32_)
    TlsFree(_key);
#else
    pthread_key_delete(_key);
#endif
  }
  
  T* get() const
  {
    T* result = NULL;
#if defined(_TTHREAD_WIN32_)
    LPVOID tmp = TlsGetValue(_key);
    result = static_cast<T*>(tmp);
#else
    void* tmp = pthread_getspecific(_key);
    result = static_cast<T*>(tmp);
#endif
    return result;
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
      T* const current_value=get();
      if(current_value!=new_value)
      {
#if defined(_TTHREAD_WIN32_)
        TlsSetValue(_key, new_value);
#else
        pthread_setspecific(_key, new_value);
#endif
      }
  }
  
};
  
}


#endif