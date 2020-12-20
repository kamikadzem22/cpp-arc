//
// Created by kamikadzem22 on 12/16/20.
//

#ifndef CPP_ARC_TYPES_H
#define CPP_ARC_TYPES_H

#include "constants.h"
typedef union AR4_header {
    struct  {
        char magic[MAGIC_SIZE];
        u_int64_t length;
    };
    char raw[AR4_HEADER_RAW_SIZE];
} AR4_header;

typedef union AR4_file_frame_header_raw {
    struct {
        u_int32_t length;
        char filename_length;
    };
    char raw[5];
} AR4_file_frame_header_raw;



/**
 * A structure with frame header data
 * */
typedef struct AR4_file_frame_header_data {
        u_int32_t length;
        char filename_size;
        char filename[MAX_NAME_LENGTH];
} AR4_file_frame_header_data;




union AR4_header* get_AR4_header();

union AR4_file_frame_header_raw* get_AR4_file_frame_header_raw(unsigned int length, char filename_size);
union AR4_file_frame_header_raw* get_AR4_file_frame_header_raw_from_bytes(char* data);
#endif //CPP_ARC_TYPES_H
