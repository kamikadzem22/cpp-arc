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

//    printf("%d", data[0]);
//    for (int i = 0; i < data_length; ++i) {
//        printf("%c", data[i]);
//    }
//    AR4_file_frame_header_raw*  frame_header = get_AR4_file_frame_header_raw_from_bytes(data);
//    printf("Frame length: %d\nName length: %ud", frame_header->length, frame_header->filename_length);
//    free(frame_header);
    FILE* kek;
    kek = fopen("text.arc", "r");
    char* data;
    int data_length;

    generate_file_frame_from_file(kek, "text.arc", get_filesize("text.arc"), &data, &data_length);
    test_print_frame_data(data, data_length);
    char* filename_parsed = get_frame_data(data, NULL);
    printf("parsed str %s, size: %zu", filename_parsed, 0);



    FILE* test;
    test = fopen("a.arc", "w");
    write_frame_to_file(test, data);

    free(data);
    free(filename_parsed);
}

int main(int argc, char *argv[]) {
    test();
//    char filename[256] = {0};
//    FILE *fp = NULL;
//
//    int help_flag = 0;
//    int extract_flag = 0;
//    int create_flag = 0;
//    int list_flag = 0;
//    int verbose_flag = 0;
//
//    int index = 0;
//    struct option longopts[] = {
//            {"help",    no_argument, &help_flag,    1},
//            {"create",  no_argument, &create_flag,  1},
//            {"extract", no_argument, &extract_flag, 1},
//            {"list",    no_argument, &list_flag,    1},
//            {"verbose", no_argument, &verbose_flag, 1},
//            {"file",    required_argument, NULL,    'f'},
//            {0, 0,                   0,             0}
//    };
//
//    int opt;
//    while ((opt = getopt_long(argc, argv, "hcxtvf:", longopts, &index)) != -1) {
//        switch (opt) {
//            case 'h':
//                help_flag = 1;
//                break;
//            case 'c':
//                create_flag = 1;
//                break;
//            case 'x':
//                extract_flag = 1;
//                break;
//            case 't':
//                list_flag = 1;
//                break;
//            case 'v':
//                verbose_flag = 1;
//                break;
//            case 'f':
//                strncpy(filename, optarg, sizeof(filename));
//                filename[sizeof(filename) - 1] = '\0';
//                break;
//            case '?':
//                usage(stderr, argv[0]);
//                return 1;
//            default:
//                break;
//
//        }
//    }
//
//
//    if (help_flag || !filename[0] || ((create_flag + extract_flag + list_flag) > 1)) {
//        usage(stdout, argv[0]);
//        return 0;
//    }
//
//    if (filename[0] && create_flag && is_writeable(filename)) {
//        fp = fopen(filename, "w");
//
//        for (int i = optind; i < argc; ++i) {
//            if (strcmp(filename, argv[i]) == 0){
//                fprintf(stderr, "Omitting archive itself: %s\n", filename);
//            }
//            if (is_readable(argv[i]))
//            {
//                FILE* file = fopen( argv[i], "r");
//                if (verbose_flag){
//                    printf("Adding file: %s (size %lu)\n", argv[i], get_filesize(file));
//                }
//            } else {
//                fprintf(stderr, "Could not access/not a file: %s\n", argv[i]);
//            }
//
//        }
//    }
//
//    if (filename[0] && extract_flag && is_readable(filename)) {
//        fp = fopen(filename, "r");
//    }
//
//    if (filename[0] && list_flag && is_readable(filename)) {
//        fp = fopen(filename, "r");
//    }
//
//    if (!fp){
//        fprintf(stderr, "Could not access file: %s\n", filename);
//        return -1;
//    }
//
//    fclose(fp);
//    return 0;
}
