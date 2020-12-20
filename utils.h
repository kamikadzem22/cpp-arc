//
// Created by kamikadzem22 on 12/16/20.
//

#include <stdio.h>

#ifndef CPP_ARC_UTILS_H
#define CPP_ARC_UTILS_H
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int is_file(char* filename);

int is_readable(char* filename);

int is_writeable(char* filename);

size_t get_filesize(char *filename);

size_t get_file_blocks_count(FILE* fp);

char * read_file_to_bytes(FILE *fp, size_t file_length);


void write_bytes_to_file(FILE *fp, char* data, size_t data_length);

#endif //CPP_ARC_UTILS_H
