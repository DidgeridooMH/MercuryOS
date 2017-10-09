#ifndef STRING_H
#define STRING_H

int     strlen(char *s);
int     strcmp(const char *dst, char *src);
int     strcpy(char *dst,const char *src);
void    strcat(void *dest,const void *src);
char*   strncpy(char *destString, const char *sourceString,int maxLength);
int     strncmp( const char* s1, const char* s2, int c );


#endif STRING_H
