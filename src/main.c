#include <Windows.h>
#include <stdio.h>

#include "utils/injector.h"

int
main (int argc, char **argv)
{
  if (argc < 2)
    {
      printf ("usage: .exe <process name> <dll name>");
      return 1;
    }

  ss_process_t process = ss_init (argv[1]);
  while (!ss_attach (&process, PROCESS_ALL_ACCESS))
    {
      printf (".");
      Sleep (1000);
    }

  printf ("\n");
  printf ("[+] found process '%s' with id %lu\n", process.name, process.pid);

  if (!ss_inject_dll (&process, argv[2]))
    {
      printf ("[-] => injection failed [%lu]\n", GetLastError ());
      ss_finish (&process);
      getchar ();
      return 1;
    }

  printf ("[+] => injected dll successfully\n");
  ss_finish (&process);
  return 0;
}
