#include <stdlib.h>
#include <stdio.h>

#include "loader.h"

char* loadTextFile(char* dir){
    FILE *file = fopen(dir, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer){
        fclose(file);
        return NULL;
    }

    size_t readSize = fread(buffer, 1, size, file);
    buffer[readSize] = '\0'; // null-terminate

    fclose(file);
    return buffer;
}
