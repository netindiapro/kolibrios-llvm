/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <libc/stubs.h>
#include <unistd.h>
#include <process.h>

int execvpe(const char *path, char * const argv[], char * const envp[])
{
  return spawnvpe(P_OVERLAY, path, argv, envp);
}
