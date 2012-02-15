#ifndef _TINYTHREAD_ONCE_H_
#define _TINYTHREAD_ONCE_H_

#define TTHREAD_ONCE_INIT  0
#define TTHREAD_ONCE_FINAL 1

namespace tthread
{
#if (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= MAC_OS_X_VERSION_10_5 || __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= __IPHONE_2_0)
  typedef int32_t once_flag;
#elif (defined(_WIN32_WINNT) && _WIN32_WINNT >= _WIN32_WINNT_WIN2K) || (defined(WINVER) && WINVER >= _WIN32_WINNT_WIN2K)
  typedef long once_flag;
#elif (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) > 40100
  typedef long once_flag;
#endif

  bool __atomic_cas(once_flag old_value,
                    once_flag new_value,
                    volatile once_flag& ref);

  inline void call_once(void(*f)(), once_flag& flag) {
    if (__atomic_cas(TTHREAD_ONCE_INIT, TTHREAD_ONCE_FINAL, flag)) {
      f();
    }
  }
}

#endif
