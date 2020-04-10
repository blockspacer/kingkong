#include "thread_util.h"
#include <boost/thread.hpp>
#ifdef OS_WIN
#include <windows.h>


#endif



BEGIN_NAMESPACE_THREAD

void SetCurrentThreadName(const std::string& thread_name) {
#ifdef OS_WIN
  // The information on how to set the thread name comes from
// a MSDN article: http://msdn2.microsoft.com/en-us/library/xcb2z8hs.aspx
  const DWORD kVCThreadNameException = 0x406D1388;

  typedef struct tagTHREADNAME_INFO {
    DWORD dwType;  // Must be 0x1000.
    LPCSTR szName;  // Pointer to name (in user addr space).
    DWORD dwThreadID;  // Thread ID (-1=caller thread).
    DWORD dwFlags;  // Reserved for future use, must be zero.
  } THREADNAME_INFO;

  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = thread_name.c_str();
  info.dwThreadID = ::GetCurrentThreadId();
  info.dwFlags = 0;

  __try {
    RaiseException(kVCThreadNameException, 0, sizeof(info) / sizeof(DWORD),
      reinterpret_cast<DWORD_PTR*>(&info));
  }
  __except (EXCEPTION_CONTINUE_EXECUTION) {
  }
#elif defined(OS_IOS) || defined (OS_OSX)
  // Mac OS X does not expose the length limit of the name, so
// hardcode it.
  const int kMaxNameLength = 63;
  std::string shortened_name = name.substr(0, kMaxNameLength);
  // pthread_setname() fails (harmlessly) in the sandbox, ignore when it does.
  // See http://crbug.com/47058
  pthread_setname_np(shortened_name.c_str());

#elif defined(OS_ANDROID) ||defined (OS_LINUX)
  prctl(PR_SET_NAME, name.c_str());
#endif
}

std::string GetCurrentThreadName() {
  return "";
}

std::string GetCurrentThreadId() {
  std::stringstream ss;
  ss << boost::this_thread::get_id();
  return ss.str();
}


END_NAMESPACE_THREAD


