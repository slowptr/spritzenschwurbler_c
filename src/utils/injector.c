#include "injector.h"

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <tlhelp32.h>

ss_process_t
ss_init (const char *name)
{
  return (ss_process_t){ 0, NULL, name };
}

void
ss_finish (ss_process_t *proc)
{
  CloseHandle (proc->handle);
}

bool
ss_attach (ss_process_t *proc, unsigned long rights)
{
  void *snap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
  if (!snap)
    {
      printf ("[-] unable to create snapshot\n");
      goto cleanup_exit_false;
    }

  PROCESSENTRY32 entry;
  entry.dwSize = sizeof (entry);

  if (!Process32First (snap, &entry))
    {
      printf ("[-] unable to gather info about processes\n");
      goto cleanup_exit_false;
    }

  do
    {
      if (strcmp (proc->name, entry.szExeFile) == 0)
        {
          proc->pid = entry.th32ProcessID;
          proc->handle = OpenProcess (rights, FALSE, proc->pid);

          CloseHandle (snap);
          return true;
        }
    }
  while (Process32Next (snap, &entry));

cleanup_exit_false:
  CloseHandle (snap);
  return false;
}

void *
ss_alloc_mem (ss_process_t *proc, SIZE_T size)
{
  return VirtualAllocEx (proc->handle, NULL, size, MEM_RESERVE | MEM_COMMIT,
                         PAGE_READWRITE);
}

void
ss_free_mem (ss_process_t *proc, void *mem)
{
  VirtualFreeEx (proc->handle, mem, 0, MEM_RELEASE);
}

void *
ss_create_remote_thread (ss_process_t *proc, void *address, void *mem)
{
  return CreateRemoteThread (proc->handle, NULL, 0,
                             (LPTHREAD_START_ROUTINE)address, mem, 0, 0);
}

bool
ss_write_mem (ss_process_t *proc, void *address, void *value, SIZE_T size)
{
  SIZE_T bytes;
  WriteProcessMemory (proc->handle, address, value, size, &bytes);
  return bytes == size;
}

bool
ss_inject_dll (ss_process_t *proc, const char *dll_path)
{
  char dll[MAX_PATH];
  if (!GetFullPathNameA (dll_path, MAX_PATH, dll, NULL))
    {
      printf ("[-] could not find dll '%s'\n", dll_path);
      return false;
    }

  void *loadlibrary
      = GetProcAddress (GetModuleHandle ("kernel32.dll"), "LoadLibraryA");
  if (!loadlibrary)
    {
      printf ("[-] unable to get loadlibrary address\n");
      goto cleanup_exit_false;
    }

  void *allocated_mem = ss_alloc_mem (proc, strlen (dll));
  if (!allocated_mem)
    {
      printf ("[-] unable to allocate memory in process\n");
      goto cleanup_exit_false;
    }

  if (!ss_write_mem (proc, allocated_mem, dll, sizeof (dll)))
    {
      printf ("[-] unable to write into allocated memory\n");
      goto cleanup_exit_false;
    }

  void *thread = ss_create_remote_thread (proc, loadlibrary, allocated_mem);
  if (!thread)
    {
      printf ("[-] unable to create remote loadlibrary thread\n");
      goto cleanup_exit_false;
    }

  Sleep (1000); // otherwise crashes..

  ss_free_mem (proc, allocated_mem);
  CloseHandle (thread);
  return true;

cleanup_exit_false:
  ss_free_mem (proc, allocated_mem);
  return false;
}
