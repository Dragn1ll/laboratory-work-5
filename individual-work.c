#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

static void join_path(char *buf, size_t buf_size,
                      const char *dir, const char *name) {
    if (snprintf(buf, buf_size, "%s/%s", dir, name) >= (int)buf_size) {
        fprintf(stderr, "Path too long: %s/%s\n", dir, name);
    }
}

static void traverse(const char *dirpath,
                     long long min_size,
                     long long max_size,
                     FILE *out,
                     long long *files_scanned) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror(dirpath);
        return;
    }

    struct dirent *entry;
    char path[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        join_path(path, sizeof(path), dirpath, name);

        struct stat st;
        if (lstat(path, &st) == -1) {
            perror(path);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            traverse(path, min_size, max_size, out, files_scanned);
        } else if (S_ISREG(st.st_mode)) {
            (*files_scanned)++;

            long long size = (long long)st.st_size;
            if (size >= min_size && size <= max_size) {
                fprintf(out, "%s\t%s\t%lld\n", path, name, size);
            }
        }
    }

    if (closedir(dir) == -1) {
        perror("closedir");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr,
                "Usage: %s <dir> <min_size> <max_size> <output_file>\n",
                argv[0]);
        return 1;
    }

    const char *start_dir = argv[1];
    long long min_size = atoll(argv[2]);
    long long max_size = atoll(argv[3]);
    const char *out_name = argv[4];

    if (min_size < 0 || max_size < 0 || min_size > max_size) {
        fprintf(stderr, "Invalid size range\n");
        return 1;
    }

    FILE *out = fopen(out_name, "w");
    if (!out) {
        perror("fopen output");
        return 1;
    }

    long long files_scanned = 0;
    traverse(start_dir, min_size, max_size, out, &files_scanned);

    if (fclose(out) == EOF) {
        perror("fclose output");
        return 1;
    }

    printf("Quantity watched files: %lld\n", files_scanned);

    return 0;
}