#include "ss_window.h"
#include "utils/kippfenster/kf_wnd.h"

kf_wnd_t ss_window;
LRESULT CALLBACK
ss_window_wndproc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
    {
    case WM_CLOSE:
      kf_wnd_destroy (&ss_window);
      return 0;
    case WM_DESTROY:
      PostQuitMessage (0);
      return 0;
    }
  return DefWindowProc (hwnd, msg, wparam, lparam);
}

bool
ss_window_init ()
{
  WNDCLASS wnd_class;
  if (!kf_wc_create (ss_window_wndproc, "ss_window", &wnd_class))
    return false;

  DWORD style = WS_OVERLAPPED | WS_SYSMENU;
  ss_window = kf_wnd_create (wnd_class, "spritzenschwurbler_c", style,
                             (int[2]){ CW_USEDEFAULT, CW_USEDEFAULT },
                             (int[2]){ 400, 150 });

  if (!kf_wnd_init (&ss_window))
    return false;

  return true;
}
int
ss_window_msg_loop ()
{
  return kf_wnd_msg_loop (&ss_window);
}
