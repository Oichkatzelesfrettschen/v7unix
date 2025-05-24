#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define PAUSESIG 15

static void waitpause(int);

s_paus(s, n)
char *s;
long int n;
{
int i;

fprintf(stderr, "PAUSE ");
if(n > 0)
	for(i = 0; i<n ; ++i)
		putc(*s++, stderr);
fprintf(stderr, " statement executed\n");
if( isatty(fileno(stdin)) )
	{
	fprintf(stderr, "To resume execution, type go.  Any other input will terminate job.\n");
	if( getchar()!='g' || getchar()!='o' || getchar()!='\n' )
		{
		fprintf(stderr, "STOP\n");
		f_exit();
		exit(0);
		}
	}
else
	{
	fprintf(stderr, "To resume execution, execute a   kill -%d %d   command\n",
		PAUSESIG, getpid() );
	signal(PAUSESIG, waitpause);
	pause();
	}
fprintf(stderr, "Execution resumes after PAUSE.\n");
}





static void
waitpause(int sig)
{
    (void)sig; /* unused parameter */
    return;
}
