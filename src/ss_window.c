#include "ss_window.h"
#include "utils/kippfenster/kf_wnd.h"
#include "utils/kippfenster/kf_wnd_ctrls.h"

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
void
ss_window_populate_items ()
{
  const int padding = 10;

  RECT rect;
  GetClientRect (ss_window.hwnd, &rect);

  int cb1_size[2] = { 222, 22 };
  kf_wnd_ctrls_add_combobox (&ss_window, "cb1",
                             (int[2]){
                                 padding,
                                 padding,
                             },
                             cb1_size);

  kf_wnd_ctrls_add_button (
      &ss_window, "btn1", "inject",
      (int[2]){ padding + cb1_size[0] + (padding / 2), padding },
      (int[2]){ 100, 22 });
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
                             (int[2]){ 365, 80 });

  if (!kf_wnd_init (&ss_window))
    return false;

  ss_window_populate_items ();

  return true;
}
int
ss_window_msg_loop ()
{
  return kf_wnd_msg_loop (&ss_window);
}
