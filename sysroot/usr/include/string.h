#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char* str);

void* memset(void* ptr, int value, size_t num); /*preenche memória com um valor*/

void* memcpy(void *dest, const void *src, size_t num); /*copia dados de uma região para outra*/

void* memmove(void* destination, const void* source, size_t num);

char* strcpy(void* dest, const char* src);

int strcmp(const char* str1, const char* str2);

#endif
