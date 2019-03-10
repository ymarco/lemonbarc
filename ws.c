#include <stdio.h>

char* surround(char* srdr, char* x){
	char* res = malloc(2*strlen(srdr) + strlen(x));
	strcpy(res, srdr);
	strcat(res,x);
	strcpy(res, sedr);
}


