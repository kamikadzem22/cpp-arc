//
// Created by kamikadzem22 on 12/16/20.
//
#include <stdlib.h>

#include "types.h"
#include "utils.h"
#include "string.h"
#include "math.h"
#include "constants.h"



union AR4_header *get_AR4_header(size_t length) {
    AR4_header* header = (AR4_header*)malloc(sizeof(AR4_header));
    strncpy(header->magic, MAGIC, MAGIC_SIZE);
    header->length = length;
    return header;
}

union AR4_file_frame_header_raw *get_AR4_file_frame_header_raw(unsigned int length, char filename_size) {
    AR4_file_frame_header_raw* header = (AR4_file_frame_header_raw*)malloc(sizeof(AR4_file_frame_header_raw));
    header->length = (int)length;
    header->filename_length = filename_size;
    return header;
}

union AR4_file_frame_header_raw *get_AR4_file_frame_header_raw_from_bytes(char *data) {
    AR4_file_frame_header_raw* header = (AR4_file_frame_header_raw*)malloc(sizeof(AR4_file_frame_header_raw));
    memcpy(header->raw, data, FRAME_HEADER_RAW_SIZE);
    return header;
}

