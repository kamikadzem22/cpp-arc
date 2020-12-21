//
// Created by kamikadzem22 on 12/16/20.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "types.h"
#include "archive.h"
#include "utils.h"

void
generate_file_frame_from_bytes(char *data, size_t data_length, char *filename, char **frame_dst, int *frame_length) {
    size_t name_length = strlen(filename);
    *frame_length = FRAME_HEADER_RAW_SIZE + name_length + data_length;


    if (name_length >= MAX_NAME_LENGTH){
        *frame_length = 0;
        *frame_dst = NULL;
        return;

    }


    *frame_dst = (char*)malloc(*frame_length);
    AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw(*frame_length, name_length);

    size_t pos = 0;
    memcpy(*frame_dst + pos, header->raw, FRAME_HEADER_RAW_SIZE);
    memcpy(*frame_dst + (pos+=FRAME_HEADER_RAW_SIZE), filename, name_length);
    memcpy(*frame_dst + (pos+=name_length), data, data_length);
    pos+=data_length;

    free(header);
}

void generate_file_frame_from_file(FILE *file, char *filename, size_t file_length, char **frame_dst, int *frame_length) {
    char* file_data;
    file_data = read_file_to_bytes(file, file_length);
    generate_file_frame_from_bytes(file_data, file_length, filename, frame_dst, frame_length);
    free(file_data);
}

void test_print_frame_data(char *frame, size_t frame_length) {
    for (size_t i = 0; i < frame_length; ++i) {
        if (isalnum(frame[i]) || frame[i] == '\n')
            printf("%c", frame[i]);
        else
            printf("\xff", frame[i]);
    }
    printf("\n");
    printf("parsing header:\n");
    AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(frame);
    printf("%d %d\n", header->length, header->filename_length);
    free(header);
}

void write_frame_to_file(FILE *file, char *frame) {
    union AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(frame);
    write_bytes_to_file(file,frame + FRAME_HEADER_RAW_SIZE + header->filename_length, get_file_in_frame_size(frame));
    free(header);
}

char *get_frame_data(char *frame, size_t *size) {
    AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(frame);
    char* filename = (char*) malloc(header->filename_length + 1);
    strncpy(filename, frame + FRAME_HEADER_RAW_SIZE, header->filename_length);
    filename[header->filename_length] = '\0';
    if (size != NULL){
        *size = header->length;
    }
    return filename;
}



size_t get_file_in_frame_size(char *frame) {
    union AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(frame);
    size_t out = header->length - FRAME_HEADER_RAW_SIZE - header->filename_length;
    free(header);
    return out;
}

size_t get_raw_frame_size(char *frame) {
    union AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(frame);
    size_t out = header->length;
    free(header);
    return out;
}


void update_archive_size(FILE *archive, int new_size) {
    fseek(archive, 0, SEEK_SET);
    AR4_header* header = get_AR4_header_raw(new_size);
    write_bytes_to_file(archive, header->raw, AR4_HEADER_RAW_SIZE);


}
//TODO: int64
u_int32_t get_archive_size(FILE *archive) {
    fseek(archive, 0, SEEK_SET);
    char* header_bytes = read_file_to_bytes(archive, AR4_HEADER_RAW_SIZE);
    union AR4_header* header =get_AR4_header_raw_from_bytes(header_bytes);
    u_int32_t out = header->length;
    free(header);
    return out;
}

void add_file_frame_to_archive(FILE *archive, char *frame_data, int frame_length) {
    fseek(archive, 0, SEEK_SET);
    u_int32_t size = get_archive_size(archive);
    fseek(archive, size, SEEK_SET);
    write_bytes_to_file(archive, frame_data, frame_length);
    size+=frame_length;
    update_archive_size(archive, size);
}

void fprint_files(FILE *stream, FILE *archive) {
    size_t pos = 0;
    size_t size = get_archive_size(archive);
    pos = AR4_HEADER_RAW_SIZE;
    while (pos < size){
        fseek(archive, pos, SEEK_SET);
        char* header_bytes = read_file_to_bytes(archive, FRAME_HEADER_RAW_SIZE);
        size_t frame_size = get_raw_frame_size(header_bytes);
        char* frame_bytes = read_file_to_bytes(archive, frame_size);
        size_t filesize;
        union AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(header_bytes);
        char* filename = get_frame_data(frame_bytes, &filesize);
        pos += frame_size;
        fprintf(stream, "%s %d bytes | namesize: %d\n", filename, filesize, header->filename_length);
        free(frame_bytes);
        free(header_bytes);
        free(header);
    }

}

void create_archive(FILE *archive) {
    AR4_header* header = get_AR4_header_raw(AR4_HEADER_RAW_SIZE);
    write_bytes_to_file(archive, header->raw, AR4_HEADER_RAW_SIZE);
    free(header);
}

void extract_files(FILE *stream, FILE *archive, int verbose_flag) {
    size_t pos = 0;
    size_t size = get_archive_size(archive);
    pos = AR4_HEADER_RAW_SIZE;
    while (pos < size){
        fseek(archive, pos, SEEK_SET);
        char* header_bytes = read_file_to_bytes(archive, FRAME_HEADER_RAW_SIZE);
        size_t frame_size = get_raw_frame_size(header_bytes);
        char* frame_bytes = read_file_to_bytes(archive, frame_size);
        size_t filesize;
        union AR4_file_frame_header_raw* header = get_AR4_file_frame_header_raw_from_bytes(header_bytes);
        char* filename = get_frame_data(frame_bytes, &filesize);
        pos += frame_size;
        if (verbose_flag)
            fprintf(stream, "Extracting file: %s\n", filename);
        FILE* file;
        if (is_file(filename)){
            fprintf(stream, "File exists: %s\n", filename);
            continue;
        }
        file = fopen(filename, "w");
        write_frame_to_file(file, frame_bytes);
        fclose(file);
        free(frame_bytes);
        free(header_bytes);
        free(header);
    }
}


