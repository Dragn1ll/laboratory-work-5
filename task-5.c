#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

static void list_dir(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    printf("Catalog contents: %s\n", path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    if (closedir(dir) == -1) {
        perror("closedir");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dir>\n", argv[0]);
        return 1;
    }

    list_dir(".");
    puts("");
    list_dir(argv[1]);

    return 0;
}