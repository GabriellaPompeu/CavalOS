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

	while(*src != '\0') *destino++ = *src++;

	*destino = '\0';
	return (char*) dest;
}

char* strncpy(void* dest, const char* src, size_t num){
	char* destino = (char*) dest;

	while(num > 0 && *src != '\0'){
		*destino++ = *src++;
		--num;
	}

	*destino++ = '\0';
	num--;

	while(num > 0){
		*destino++ = '\0';
		--num;
	} 

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

int strncmp(const char* a, const char* b, size_t num){
	int res = 0;

	while(num > 0){
		if((res = *a++ - *b++) != 0) return res;
		--num;
	}
	return 0;
}

void* memmove(void* dest, const void* src, size_t num){
	char* d = (char*) dest;
	const char* origem = (char*) src;

	if(dest <= src){
		while(num > 0){
			*d++ = *origem++;
			--num;
		}
	}else{
		d += num - 1;
		origem += num - 1;
		while(num > 0){
			*d-- = *origem--;
			--num;
		}
	}
	return dest;
}

int memcmp(const void* str1, const void* str2, size_t num){
	const unsigned char* a = (const unsigned char*) str1, *b = (const unsigned char*) str2;
	int res = 0;

	while(num > 0){
		if((res = *a - *b) != 0) return res;
		a++; b++;
		--num;
	}
	return 0;
}

int strncasecmp(const char* str1, const char* str2, size_t num){
	//Yes, Maria Clara, it had better be nice with your friends
	char a, b;
	int res = 0;

	while(num > 0){
		a = *str1++;
		b = *str2++;
		if(a == '\0' && b == '\0') return 0;
		if(a >= 'A' && a <= 'Z') a += 'a' - 'A';
		if(b >= 'A' && b <= 'Z') b += 'a' - 'A';
		if((res = a - b) != 0) return res;
		--num;
	}
	return 0;
}

int strcasecmp(const char* str1, const char* str2){
	char a = *str1++, b = *str2++;
	int res = 0;

	while(a != '\0' || b != '\0'){
		if(a >= 'A' && a <= 'Z') a += 'a' - 'A';
		if(b >= 'A' && b <= 'Z') b += 'a' - 'A';
		if((res = a - b) != 0) return res;
		a = *str1++;
		b = *str2++;
	}
	return a - b;
}

char* strchr(const char* str, int c){
	while(*str != (char)c){
		if(*str == '\0') return NULL;
		str++;
	}
	return (char*)str;
}

char* strrchr(const char* str, int c){
	char* tmp = NULL, valor = (char) c;

	while(*str != '\0'){
		if(*str == valor) tmp = str;
		str++;
	}
	if(valor == '\0') return str;
	return tmp;
}

char* strstr(const char* str1, const char* str2){
	if(*str2 == '\0') return str1;

	char* a = str1, *b = str2;

	while(*str1 != '\0'){
		if(*str1 != *b) str1++;

		if(*str2 == *str1){
			a = str1;
			while(*str2 == *str1){
				str1++; str2++;
				if(*str2 == '\0') return a;
				if(*str1 == '\0') return NULL;
			}

			str1 = ++a; str2 = b;
		}
	}
	return NULL;
}

char* strcat(char* dest, char* src){
	char* tmp = dest;

	while(*dest != '\0') dest++;
	while(*src != '\0') *dest++ = *src++;
	*dest = '\0';
	return tmp;
}

char* strncat(char* dest, char* src, size_t num){
	char* tmp = dest;

	while(*dest != '\0') dest++;
	while(num > 0 && *src != '\0'){
		*dest++ = *src++;
		--num;
	}
	*dest = '\0';
	return tmp;
}
