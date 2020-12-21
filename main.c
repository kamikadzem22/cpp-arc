//
// Created by kamikadzem22 on 12/15/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>


#include "types.h"
#include "utils.h"
#include "archive.h"

#include <errno.h>
#include <string.h>


void usage(FILE *fp, const char *path) {
    const char *basename = strrchr(path, '/');
    basename = basename ? basename + 1 : path;


    fprintf(fp, "usage: %s [OPTION...] [FILE...]"
                "\n", basename);

    fprintf(fp, "@kamikadzem22 edition 'arc' saves many files together into a single disk archive, and can\n"
                "    restore files back from the archive."
                "\n");


    fprintf(fp, "  -h, --help\t\t\t"
                "give this help list\n");
    fprintf(fp, "  -f, --file[=FILENAME]\t\t"
                "use archive file\n");
    fprintf(fp, "  -c, --create\t\t\t"
                "create a new archive\n");
    fprintf(fp, "  -x, --extract\t\t\t"
                "extract files from an archive\n");
    fprintf(fp, "  -t, --list\t\t\t"
                "list the contents of an archive\n");
    fprintf(fp, "  -v, --verbose\t\t\t"
                "verbosely list files processed\n");


}

void test(){

//    printf("%d", frame_data[0]);
//    for (int i = 0; i < data_length; ++i) {
//        printf("%c", frame_data[i]);
//    }
//    AR4_file_frame_header_raw*  frame_header = get_AR4_file_frame_header_raw_from_bytes(frame_data);
//    printf("Frame length: %d\nName length: %ud", frame_header->length, frame_header->filename_length);
//    free(frame_header);
    FILE* kek;
    kek = fopen("text.arc", "rb");
    char* frame_data;
    int data_length;

    generate_file_frame_from_file(kek, "text.arc", get_filesize("text.arc"), &frame_data, &data_length);
    test_print_frame_data(frame_data, data_length);
    char* filename_parsed = get_frame_data(frame_data, NULL);
    printf("parsed str %s, size: %zu\n", filename_parsed, 0);

    FILE* archive;
    delete_file("new.arc");
    archive = fopen("new.arc", "wb+");

    create_archive(archive);

    FILE* kek1;
    kek1 = fopen("test.arc", "rb");
    char* newframe = read_file_to_bytes(kek1, get_filesize("cpp_arc"));
    int new_size;
    generate_file_frame_from_bytes(newframe, get_filesize("cpp_arc"), "cpp_arc", &newframe, &new_size);

    add_file_frame_to_archive(archive, frame_data, data_length);
    add_file_frame_to_archive(archive, frame_data, data_length);
    add_file_frame_to_archive(archive, newframe, new_size);
    add_file_frame_to_archive(archive, frame_data, data_length);
    add_file_frame_to_archive(archive, newframe, new_size);


    printf("\n");
    fprint_files(stdout, archive);
    fclose(archive);


//
//    FILE* test;
//    test = fopen("a.arc", "w");
//    write_frame_to_file(test, frame_data);
//
//    free(frame_data);
//    free(filename_parsed);
}

int main(int argc, char *argv[]) {
//    test();
    char filename[256] = {0};
    FILE *fp = NULL;

    int help_flag = 0;
    int extract_flag = 0;
    int create_flag = 0;
    int list_flag = 0;
    int verbose_flag = 0;

    int index = 0;
    struct option longopts[] = {
            {"help",    no_argument, &help_flag,    1},
            {"create",  no_argument, &create_flag,  1},
            {"extract", no_argument, &extract_flag, 1},
            {"list",    no_argument, &list_flag,    1},
            {"verbose", no_argument, &verbose_flag, 1},
            {"file",    required_argument, NULL,    'f'},
            {0, 0,                   0,             0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hcxtvf:", longopts, &index)) != -1) {
        switch (opt) {
            case 'h':
                help_flag = 1;
                break;
            case 'c':
                create_flag = 1;
                break;
            case 'x':
                extract_flag = 1;
                break;
            case 't':
                list_flag = 1;
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case 'f':
                strncpy(filename, optarg, sizeof(filename));
                filename[sizeof(filename) - 1] = '\0';
                break;
            case '?':
                usage(stderr, argv[0]);
                return 1;
            default:
                break;

        }
    }


    if (help_flag || !filename[0] || ((create_flag + extract_flag + list_flag) > 1)) {
        usage(stdout, argv[0]);
        return 0;
    }

    if (filename[0] && create_flag && is_writeable(filename)) {
        delete_file(filename);
        fp = fopen(filename, "w+");
        create_archive(fp);
        for (int i = optind; i < argc; ++i) {
            if (strcmp(filename, argv[i]) == 0){
                fprintf(stderr, "Omitting archive itself: %s\n", filename);
            }
            if (is_readable(argv[i]))
            {
                if (verbose_flag){
                    printf("Adding file: %s\n", argv[i]);
                }

                FILE* file;
                file = fopen(argv[i], "r");
                size_t filesize = get_filesize(argv[i]);
                char* file_bytes;
                char* frameData = read_file_to_bytes(file, filesize);
                int newFileSize;
                generate_file_frame_from_bytes(frameData, filesize, argv[i], &frameData, &newFileSize);
                add_file_frame_to_archive(fp, frameData, newFileSize);
                free(frameData);
                free(file_bytes);
                fclose(file);


            } else {
                fprintf(stderr, "Could not access/not a file: %s\n", argv[i]);
            }
            printf("archive size: %ld\n", get_archive_size(fp));
        }
    }

    if (filename[0] && extract_flag && is_readable(filename)) {
        fp = fopen(filename, "r");

        extract_files(stdout, fp, verbose_flag);
    }

    if (filename[0] && list_flag && is_readable(filename)) {
        fp = fopen(filename, "rb");
        fprint_files(stdout, fp);
    }

    if (!fp){
        fprintf(stderr, "Could not access file: %s\n", filename);
        return -1;
    }

    fclose(fp);
    return 0;
}
