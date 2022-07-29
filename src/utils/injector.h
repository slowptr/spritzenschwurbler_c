#ifndef SPRITZENSCHWURBLER_C_INJECTOR_H
#define SPRITZENSCHWURBLER_C_INJECTOR_H

#include <basetsd.h>
#include <stdbool.h>

#include "process_t.h"

ss_process_t ss_init (const char *name);

void ss_finish (ss_process_t *proc);

bool ss_attach (ss_process_t *proc, unsigned long rights);

void *ss_alloc_mem (ss_process_t *proc, SIZE_T size);

void ss_free_mem (ss_process_t *proc, void *mem);

void *ss_create_remote_thread (ss_process_t *proc, void *address, void *mem);

bool ss_write_mem (ss_process_t *proc, void *address, void *value,
                   SIZE_T size);

bool ss_inject_dll (ss_process_t *proc, const char *dll_path);

#endif // SPRITZENSCHWURBLER_C_INJECTOR_H
