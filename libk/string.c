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