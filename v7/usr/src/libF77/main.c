/* STARTUP PROCEDURE FOR UNIX FORTRAN PROGRAMS */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int xargc;
char **xargv;

static void sigfdie();
static void sigidie();
static void sigdie(char *s);

main(argc, argv, arge)
int argc;
char **argv;
char **arge;
{

xargc = argc;
xargv = argv;
signal(SIGFPE, sigfdie);	/* ignore underflow, enable overflow */
signal(SIGIOT, sigidie);
MAIN__();
f_exit();
}


static sigfdie()
{
sigdie("Floating Exception");
}



static sigidie()
{
sigdie("IOT Trap");
}



static sigdie(s)
register char *s;
{
/* print error message, then clear buffers */
fflush(stderr);
fprintf(stderr, "%s\n", s);
f_exit();
fflush(stderr);

/* now get a core */
signal(SIGIOT, 0);
abort();
}
