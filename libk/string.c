#include <stdbool.h>
#include <string.h>
#include <stdint.h> 

size_t strlen(const char* str){
	size_t len = 0;
	while (str[len]) len++;
	return len;
}

void* memset(void* ptr, int value, size_t num){
	uint8_t* destino = (uint8_t*)ptr;
	
	for (size_t i = 0; i < num; i++){
		*destino = value;
		destino++;
	}

	return ptr;
}

void* memcpy(void *dest, const void *src, size_t num){
	uint8_t *destino = (uint8_t *)dest;
	const uint8_t *origem = (const uint8_t *)src;

	for (size_t i = 0; i < num; i++){
		*destino = *origem;
		destino++; origem++;
	}

	return dest;
}

char* strcpy(void* dest, const char* src){
	char* destino = (char*) dest;
	size_t i = 0;

	while(src[i] != '\0'){
		*destino = src[i];
		destino++;
		i++;
	}

	*destino = src[i];
	return (char*) dest;
}

int strcmp(const char* str1, const char* str2){
	char a = *str1, b = *str2;

	while((a == b) && a != '\0'){
		a = *(++str1);
		b = *(++str2);
	}

	if(a == '\0' && b == '\0') return 0;
	return 1;
}
