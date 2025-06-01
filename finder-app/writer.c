// finder-app/writer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file_path> <text_string>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    fprintf(fp, "%s", argv[2]);
    fclose(fp);
    return 0;
}

