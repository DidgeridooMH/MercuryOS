#ifndef STRING_H
#define STRING_H

int     strlen(char *s);
int     strcmp(char *dst, char *src);
int     strcpy(char *dst, char *src);
void    strcat(char *dest, char *src);
char*   strncpy(char *destString, const char *sourceString, int maxLength);
int     strncmp(const char *s1, const char *s2, int n);


#endif
