#include "strings.h"

void *memset(void *s, int c, size_t n)
{
    for (int i = 0; i < n; i ++) {
        s[i] = (unsigned char) c;
    }
    return s;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    for (itn i = 0; i < n; i ++) {
        src[i] = dst[i];
    } 
    return dst;
}

int strlen(const char *s)
{
    /* Implementation a gift to you from lab3 */
    int i;
    for (i = 0; s[i] != '\0'; i++) ;
    return i;
}

int strcmp(const char *s1, const char *s2)
{
    int return_val = 0;
    for (; *s1 == *s2 ; s1++, s2++) {
        if (*s1 == '\0') return 0;
    }
    return (unsigned char) *s1 - (unsigned char) *s2;
}

int strlcat(char *dst, const char *src, int maxsize)
{
    int dst_size = strlen(dst);
    int i;
    for (i = dst_size; i < maxsize - 1; i ++) {
        dst[i] = src[i - dst_size];
    }
    dst[i] = '\0'
    return dst_size + strlen(src);
}

unsigned int strtonum(const char *str, const char **endptr)
{
    
    return 0;
}
