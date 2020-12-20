//
// Created by kamikadzem22 on 12/16/20.
//

#ifndef CPP_ARC_ARCHIVE_H
#define CPP_ARC_ARCHIVE_H
#include <stdio.h>
#include "types.h"

//TODO: main function to archive given set of data
/**
 * @param archive file pointer to archive
 * @param file file pointer to file
 * @param filename
 * high level function
 * works on correct archive
 * parses headers and finds last piece of data to write file
 * */
void add_file_to_archive(FILE* archive, FILE* file, char* filename);

/**
 * @brief creates bytes ready to be written to archive
 * @param data array of data bytes
 * @param data_length length of data
 * @param filename name of the file, not more than 255 chars
 * @param frame_length reference which is filled with frame length
 * @param frame_dst reference which is used to write pointer to access generated frame
 *
 * frame_dst results in:
 * @code
 * +-------------+---------------+-------------------+--------------------+
 * |  Frame size | Filename size |     Filename      |        Data        |
 * |  (4 bytes)  |   (1 byte)    | (up to 255 bytes) | (up to 2^32 bytes) |
 * +-------------+---------------+-------------------+--------------------+
 *
 * Frame structure
 * */
void generate_file_frame_from_bytes(char *data, size_t data_length, char *filename, char **frame_dst, int *frame_length);

/**
 * @brief uses generate_file_frame_from_bytes after file is read
 * @param file file pointer used to read data
 * @param filename name of the file, not more than 255 chars
 * @param file_length length of the file that would be read
 * @param frame_length reference which is filled with frame length
 * @param frame_dst reference which is used to write pointer to access generated frame
 * @see generate_file_frame_from_bytes()
 * */
void generate_file_frame_from_file(FILE *file, char *filename, size_t file_length, char **frame_dst, int *frame_length);

/**
 * @brief writes given frame as a normal file
 * @param file file pointer used to write
 * @param frame frame data
 * @param frame_length
 * */
void write_frame_to_file(FILE *file, char *frame);

/**
 * @brief returns filename
 * @param size (optional) gets filled with filesize in archive
 * */
char* get_frame_data(char *frame, size_t *size);

size_t get_file_in_frame_size(char* frame);

void test_print_frame_data(char* frame, size_t frame_length);

//TODO:
void decode_file_frame_from_bytes(char* frame, size_t frame_length);

//TODO: write given frame to file
void add_file_frame_to_archive(FILE* archive, char* frame_data, int frame_length);

//TODO: update archive size after file write
void update_archive_size(FILE* archive, int new_size, int size_position);

//TODO:
void create_archive(FILE* archive);

//TODO: check if ar4 file
int is_ar4_arc(FILE* fp);
#endif //CPP_ARC_ARCHIVE_H
