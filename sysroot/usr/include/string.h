#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char* str);

void* memset(void* ptr, int value, size_t num); /*preenche memória com um valor*/

void* memcpy(void *dest, const void *src, size_t num); /*copia dados de uma região para outra*/

void* memmove(void* destination, const void* source, size_t num);

char* strcpy(void* dest, const char* src);

char* strncpy(void* dest, const char* src, size_t num);

int strcmp(const char* str1, const char* str2);

int strncmp(const char* a, const char* b, size_t num);

int memcmp(const void* str1, const void* str2, size_t num);

int strncasecmp(const char* str1, const char* str2, size_t num);

int strcasecmp(const char* str1, const char* str2);

char* strchr(const char* str, int c);

char* strrchr(const char* str, int c);

char* strstr(const char* str1, const char* str2);

char* strcat(char* dest, char* src);

char* strncat(char* dest, char* src, size_t num);

size_t strspn(const char* str1, const char* str2);

size_t strcspn(const char* str1, const char* str2);

char* strpbrk(const char* str1, const char* str2);

char* strtok(char* original, const char* delimitadores);

char* strlwr(char* str);

char* strupr(char* str);

char* stpcpy(char* dest, const char* str);

int atoi(const char* str);

void* memchr(const void* ptr, int value, size_t num);

#endif
