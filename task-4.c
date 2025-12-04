#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }

    const char *src = argv[1];
    const char *dst = argv[2];

    struct stat st;
    if (stat(src, &st) == -1) {
        perror("stat");
        return 1;
    }

    FILE *fs = fopen(src, "r");
    if (!fs) {
        perror("fopen src");
        return 1;
    }

    FILE *fd = fopen(dst, "w");
    if (!fd) {
        perror("fopen dst");
        fclose(fs);
        return 1;
    }

    int ch;
    while ((ch = fgetc(fs)) != EOF) {
        if (fputc(ch, fd) == EOF) {
            perror("fputc");
            fclose(fs);
            fclose(fd);
            return 1;
        }
    }

    if (ferror(fs)) {
        perror("fgetc");
        fclose(fs);
        fclose(fd);
        return 1;
    }

    if (fclose(fs) == EOF) {
        perror("fclose src");
        fclose(fd);
        return 1;
    }
    if (fclose(fd) == EOF) {
        perror("fclose dst");
        return 1;
    }

    if (chmod(dst, st.st_mode & 07777) == -1) {
        perror("chmod");
        return 1;
    }

    return 0;
}