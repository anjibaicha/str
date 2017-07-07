#include "fatal_error.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * show an error message on stderr on terminate execution
 */
void
fatal_error(const char *message)
{
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}
