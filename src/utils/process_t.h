#ifndef SPRITZENSCHWURBLER_C_PROCESS_T_H
#define SPRITZENSCHWURBLER_C_PROCESS_T_H

typedef struct ss_process_s
{
  unsigned long pid;
  void *handle;
  const char *name;
} ss_process_t;

#endif // SPRITZENSCHWURBLER_C_PROCESS_T_H
