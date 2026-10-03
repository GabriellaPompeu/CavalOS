#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define FS_MAX_NAME 32
#define FS_MAX_FILES 16
#define FS_MAX_FILE_SIZE 4096

typedef struct {
    char name[FS_MAX_NAME];
    uint8_t data[FS_MAX_FILE_SIZE];
    size_t size;
    bool used;
}fs_file;

typedef struct {
    fs_file files[FS_MAX_FILES];
    size_t file_count;
} filesystem;

void filesystem_init(filesystem* fs);

fs_file* filesystem_create_file(filesystem* fs, const char* name);

fs_file* filesystem_find_file(filesystem* fs, const char* name);

bool filesystem_write_file(fs_file* file, const uint8_t* data, size_t size);

size_t filesystem_read_file(fs_file* file, uint8_t* buffer, size_t size);

bool filesystem_delete_file(filesystem* fs, const char* name);

void filesystem_list_files(filesystem* fs);

#endif
