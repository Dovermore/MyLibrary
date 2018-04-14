/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: dynamic_getword.h
 
 *This program is created for the purpose of:
    
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


#define TRUE 1
#define FALSE 0

int getword(char **W, int n) {
    int c, len=0, mlen = n;
    /* first, skip over any non alphabetics */
    while ((c=getchar())!=EOF && !isalnum(c)) {
        /* do nothing more */
    }
    if (c==EOF) {
        return EOF;
    }
    /* ok, first character of next word has been found */
    *W[len++] = c;
    while ((c=getchar())!=EOF && isalnum(c)) {
        /* another character to be stored */
        (*W)[len++] = c;
        mlen = checkndoublef(W, mlen);
    }
    /* now close off the string */
    (*W)[len] = '\0';
    return mlen;
}

int mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}

char *str_reallocf(char **pstr, int len) {
    assert(len == strlen(*pstr) + 1);
    *pstr = (char*)realloc(*pstr, 2 * len * sizeof(char));
    assert(*pstr != NULL);
    return *pstr;
}


int checkndoublef(char **pstr, int len) {
    if (strlen(*pstr) + 1 == len) {
        str_reallocf(pstr, len);
        return 2 * len;
    }
    return len;
}