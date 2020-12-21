//
// Created by kamikadzem22 on 12/16/20.
//
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "utils.h"

int is_file(char *filename) {
    return access(filename, F_OK) == 0;
}

int is_readable(char *filename) {
    return access(filename, R_OK) == 0;
}

int is_writeable(char *filename) {
    return access(filename, W_OK) == 0;
}

size_t get_file_blocks_count(FILE *fp) {
    if (!fp){
        return 0;
    }
    int file_pos = ftell(fp);
    int length;
    fseek(fp, 0L, SEEK_SET);
    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    fseek(fp, file_pos, SEEK_SET);

    return length;
}

char * read_file_to_bytes(FILE *fp, size_t file_length) {
    unsigned int file_pos = ftell(fp);
    if (fp == NULL){
        return NULL;
    }
    char* data = (char*)malloc(file_length);
    fread(data, file_length, 1, fp);
    fseek(fp, file_pos, SEEK_SET);
    return data;
}

size_t get_filesize(char *filename) {
    struct stat sb;

    if (stat(filename, &sb) == -1) {
        perror("stat");
    }

    return  sb.st_size;
}

void write_bytes_to_file(FILE *fp, char *data, size_t data_length) {
    fwrite(data, data_length, 1, fp);
}

void delete_file(char *filename) {
    FILE* fp;
    fp = fopen(filename, "w");
    fclose(fp);
}
