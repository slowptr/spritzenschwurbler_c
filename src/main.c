#include <Windows.h>
#include <stdio.h>

#include "ss_window.h"

int APIENTRY
WinMain (HINSTANCE inst, HINSTANCE prev_inst, PSTR cmdline, int cmdshow)
{
  if (!ss_window_init ())
    {
      fprintf (stderr, "Failed to create window.\n");
      return 1;
    }

  return ss_window_msg_loop ();
}
