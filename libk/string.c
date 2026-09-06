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

/* strcpy from Maria Clara

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
*/

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

	while(num > 0){
		*destino++ = '\0';
		--num;
	} 

	return (char*) dest;
}

int strcmp(const char* str1, const char* str2){
	const unsigned char* a = (const unsigned char*) str1, *b = (const unsigned char*) str2;

	while((*a == *b) && *a != '\0'){ a++; b++;}

	return (int)*a - (int)*b;
}

int strncmp(const char* str1, const char* str2, size_t num){
	const unsigned char* a = (const unsigned char*) str1;
	const unsigned char* b = (const unsigned char*) str2;

	while(num > 0){
		if(*a != *b) return (int)*a - (int)*b;
		if(*a == '\0') return 0;
		a++; b++; --num;
	}
	return 0;
}

void* memmove(void* dest, const void* src, size_t num){
	char* d = (char*) dest;
	const char* origem = (const char*) src;

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
	const char* tmp = NULL;
	char valor = (char) c;

	while(*str != '\0'){
		if(*str == valor) tmp = str;
		str++;
	}
	if(valor == '\0') return (char*)str;
	return (char*)tmp;
}

char* strstr(const char* str1, const char* str2){
	if(*str2 == '\0') return (char*)str1;

	const char* a = str1, *b = str2;

	while(*str1 != '\0'){
		if(*str1 != *b) str1++;

		if(*str2 == *str1){
			a = str1;
			while(*str2 == *str1){
				str1++; str2++;
				if(*str2 == '\0') return (char*)a;
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

size_t strspn(const char* str1, const char* str2){
	const char* tmp = str2;
	size_t cont = 0;

	while(*str1 != '\0'){
		while(*str2 != *str1 && *str2 != '\0') str2++;
		if(*str2 == '\0') return cont;
		++cont;
		str1++;
		str2 = tmp;
	}

	return cont;
}

size_t strcspn(const char* str1, const char* str2){
	const char* tmp = str2;
	size_t cont = 0;

	while(*str1 != '\0'){
		while(*str2 != *str1 && *str2 != '\0') str2++;
			if(*str2 == *str1) return cont;
			++cont;
			str1++;
			str2 = tmp;
	}
	return cont;
}

char* strpbrk(const char* str1, const char* str2){
	const char* tmp = str2;

	while(*str1 != '\0'){
		while(*str2 != *str1 && *str2 != '\0') str2++;
		if(*str1 == *str2) return (char*)str1;
		str1++;
		str2 = tmp;
	}
	return NULL;
}

char* strtok(char* original, const char* delimitadores){
    static char* tmp;
    const char* delim;
    char* token_achado;

    // Se uma nova string for passada, reinicia o ponteiro estático
    if (original)
        tmp = original;
    else
        original = tmp;

    // Pula delimitadores consecutivos que estejam no início do token
    while (*original != '\0'){
        delim = delimitadores;

        while (*delim != '\0' && *original != *delim)
            delim++;

        if (*original != *delim)
            break;

        original++;
    }

    if (*original == '\0'){
        tmp = original;
        return NULL;
    }

    // Guarda o início do token válido
    token_achado = original;

    // Percorre o token até encontrar o próximo delimitador ou o fim da string
    while (*original != '\0'){
        delim = delimitadores;

        while (*delim != '\0' && *original != *delim)
            delim++;

        if (*original == *delim){
            *original = '\0';
            tmp = original + 1;
            return token_achado;
        }

        original++;
    }

    // Se chegou ao fim da string (encontrou o último token)
    tmp = original;
    return token_achado;
}

char* strlwr(char* str){
	char* tmp = str;

	while(*str != '\0'){
		if(*str >= 'A' && *str <= 'Z') *str += 'a' - 'A';
		str++;
	}
	return tmp;
}

char* strupr(char* str){
	char* tmp = str;

	while(*str != '\0'){
		if(*str >= 'a' && *str <= 'z') *str = *str - ('a' - 'A');
		str++;
	}
	return tmp;
}

char* stpcpy(char* dest, const char* str){
	while(*str != '\0') *dest++ = *str++;
	*dest = '\0';
	return dest;
}

//Tem que ver como que o atoi vai tratar a questão de negativos e caracteres.
int atoi(const char* str){
	int res = 0;
	int negativo = 0;
	while(*str < '0' || *str > '9'){
		if(*str == '-') negativo++;
		str++;
	}

	while(*str >= '0' && *str <= '9'){
		res = res * 10 + (*str - '0');
		str++;
	}
	if(negativo) return -res;
	return res;
}

void* memchr(const void* ptr, int value, size_t num){
    const unsigned char* str = ptr;

    while(num--){
        if(*str == (unsigned char)value)
            return (void*)str;

        str++;
    }

    return NULL;
}
