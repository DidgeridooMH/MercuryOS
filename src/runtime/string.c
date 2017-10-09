#include "string.h"

int strlen(char *s) {
  int n = 0;
  while(*s++) {
    n++;
  }
  return n;
}

int strcmp(const char *dst, char *src) {
  int n = strlen(dst);
  for(int i = 0; i < n; i++) {
    if(src[i] != dst[i]) {
      if(src[i] > dst[i]) {
        return -1;
      } else {
        return 1;
      }
    }
  }
  return 0;
}

int strcpy(char *dst, const char *src) {
  int n = strlen(src);
  for(int i = 0; i < n; i++) {
    dst[i] = src[i];
  }
  return 0;
}

void strcat(void *dest,const void *src) {
  int n = strlen(dest);
  int m = strlen(src);
  for(int i = 0; i < m; i++) {
    dest[n + i] = src[i];
  }
  dest[n + m] = '\0';
  return;
}

int strncmp( const char* s1, const char* s2, int c ) {
	int result = 0;

	while ( c ) {
		result = *s1 - *s2++;

		if ( ( result != 0 ) || ( *s1++ == 0 ) ) {
			break;
		}

		c--;
	}

	return result;
}

char *strncpy(char *destString, const char *sourceString,int maxLength)
{
  unsigned count;

  if ((destString == (char *) NULL) || (sourceString == (char *) NULL))
	{
	  return (destString = NULL);
	}

  if (maxLength > 255)
	maxLength = 255;

  for (count = 0; (int)count < (int)maxLength; count ++)
	{
	  destString[count] = sourceString[count];

	  if (sourceString[count] == '\0')
	break;
	}

  if (count >= 255)
	{
	  return (destString = NULL);
	}

  return (destString);
}
