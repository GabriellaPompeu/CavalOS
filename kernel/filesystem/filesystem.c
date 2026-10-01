#include "filesystem.h"
#include <string.h>

void filesystem_init(filesystem* fs){
    fs->file_count = 0;

    for (size_t i = 0; i < FS_MAX_FILES; i++) {
        fs->files[i].used = false;
        fs->files[i].size = 0;
        fs->files[i].name[0] = '\0';
    }
}

fs_file* filesystem_find_file(filesystem* fs, const char* name){
    for (size_t i = 0; i < FS_MAX_FILES; i++) {
        if (!fs->files[i].used)
            continue;

        if (strcmp(fs->files[i].name, name) == 0)
            return &fs->files[i];
    }

    return NULL;
}

fs_file* filesystem_create_file(filesystem* fs, const char* name){
    if (fs->file_count >= FS_MAX_FILES)
        return NULL;

    if (filesystem_find_file(fs, name) != NULL)
        return NULL;

    for (size_t i = 0; i < FS_MAX_FILES; i++) {

        if (fs->files[i].used)
            continue;

        fs->files[i].used = true;
        fs->files[i].size = 0;

        strcpy(fs->files[i].name, name);

        fs->file_count++;

        return &fs->files[i];
    }

    return NULL;
}

bool filesystem_write_file(fs_file* file, const uint8_t* data, size_t size){
    if (file == NULL)
        return false;

    if (!file->used)
        return false;

    if (size > FS_MAX_FILE_SIZE)
        return false;

    memcpy(file->data, data, size);

    file->size = size;

    return true;
}

size_t filesystem_read_file(fs_file* file, uint8_t* buffer, size_t size){
    if (file == NULL)
        return 0;

    if (!file->used)
        return 0;

    if (size > file->size)
        size = file->size;

    memcpy(buffer, file->data, size);

    return size;
}