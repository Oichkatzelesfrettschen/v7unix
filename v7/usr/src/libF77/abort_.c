#include <stdio.h>
#include <stdlib.h>

abort_()
{
fprintf(stderr, "Fortran abort routine called\n");
_cleanup();
abort();
}
