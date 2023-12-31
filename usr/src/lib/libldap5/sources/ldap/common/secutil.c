/*
 * Copyright (c) 2001 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char	hexdig[] = "0123456789abcdef";

char* hexa_print(char *aString, int aLen)
{
	char *res;
	int i =0;

	if ((res = (char *)calloc (aLen*2 + 1, 1 )) == NULL){
		return (NULL);
	}
	for (;;){
		if (aLen < 1)
			break;
		res[i] = hexdig[ ( *aString & 0xf0 ) >> 4 ];
		res[i + 1] = hexdig[ *aString & 0x0f ];
		i+= 2;
		aLen--;
		aString++;
	}
	return (res);
}
