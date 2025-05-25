%{
#include <stdio.h>
%}
%%
input:
    /* empty */ { printf("Sample parser ran\n"); }
;
%%
int yyerror(const char *s) { fprintf(stderr, "%s\n", s); return 0; }

