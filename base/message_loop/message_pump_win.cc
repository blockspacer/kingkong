#include "message_pump_win.h"
#include "log.h"
#include "time_util.h"
#include "thread_util.h"
#include <boost/assert.hpp>
#include <boost/timer.hpp>
#include <windows.h>
BEGIN_NAMESPACE_LOOPER

#define MESSAGE_PUMP_WIN_CLASS "MessagePumpWin"
#define MESSAGE_PUMP_TIMER_ID 1
#define MESSAGE_PUMP_CUSTOM (WM_USER + 1)

HWND g_hwnd_ = nullptr;

static LRESULT CALLBACK WndProcThunk(HWND hwnd,
                                     UINT message,
                                     WPARAM wparam,
                                     LPARAM lparam) {
  if (message == MESSAGE_PUMP_CUSTOM) {
    // windows 会有domodel，所以只能做一个任务
    MessagePumpWin* msg_wnd = reinterpret_cast<MessagePumpWin*>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));
    msg_wnd->Work();
    return 0;
  }
  return ::DefWindowProc(hwnd, message, wparam, lparam);
}

static VOID CALLBACK WndTimerProcThunk(HWND hwnd,
                                     UINT message,
                                     UINT_PTR timer_id,
                                     DWORD now) {
  KillTimer(hwnd, timer_id);
  //windows 会有domodel，所以只能做一个任务
  MessagePumpWin* msg_wnd = reinterpret_cast<MessagePumpWin*>(
      GetWindowLongPtr(hwnd, GWLP_USERDATA));
  msg_wnd->Work();
}

MessagePumpWin::MessagePumpWin() : MessagePumpImpl("Main") {
  BASE_THREAD::SetThreadTls(this);
  BASE_THREAD::SetCurrentThreadName(name());
  WNDCLASSEXA window_class = { 0 };
  window_class.cbSize = sizeof(window_class);
  window_class.lpfnWndProc = WndProcThunk;
  window_class.hInstance = ::GetModuleHandle(nullptr);
  window_class.lpszClassName = MESSAGE_PUMP_WIN_CLASS;
  RegisterClassExA(&window_class);

  g_hwnd_ = CreateWindowA(MESSAGE_PUMP_WIN_CLASS, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, window_class.hInstance, nullptr);
  SetWindowLongPtr(g_hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

MessagePumpWin::~MessagePumpWin() {
}

void MessagePumpWin::Work() {
  DoOneWork();
}

void MessagePumpWin::Wakeup(uint64_t expired_time) {
  auto now = BASE_TIME::GetTickCount2();
  if (expired_time > now) {
    ::SetTimer(g_hwnd_, MESSAGE_PUMP_TIMER_ID, static_cast<UINT>(expired_time - now), WndTimerProcThunk);
  } else {
    ::PostMessage(g_hwnd_, MESSAGE_PUMP_CUSTOM, 0, 0);
  }
}

void MessagePumpWin::DoRun() {
}

void MessagePumpWin::DoStop() {
  if (nullptr != g_hwnd_) {
    ::DestroyWindow(g_hwnd_);
    g_hwnd_ = nullptr;
  }
}


END_NAMESPACE_LOOPER
