#ifndef STRING_H
#define STRING_H

int     strlen(char *s);
int     strcmp(const char *dst, char *src);
int     strcpy(char *dst,const char *src);
void    strcat(char *dest,const char *src);
char*   strncpy(char *destString, const char *sourceString,int maxLength);


#endif STRING_H
