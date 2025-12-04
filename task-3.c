#include <stdio.h>
#include <stdlib.h>

static void wait_key(void) {
    int c;
    printf("\n Press Enter to continue");
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <N>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    long N = strtol(argv[2], NULL, 10);
    if (N < 0) {
        fprintf(stderr, "N must be >= 0\n");
        return 1;
    }

    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    int ch;
    long line_count = 0;

    while ((ch = fgetc(f)) != EOF) {
        putchar(ch);
        if (ch == '\n') {
            line_count++;
        }

        if (N > 0 && line_count >= N) {
            wait_key();
            line_count = 0;
        }
    }

    if (ferror(f)) {
        perror("fgetc");
        fclose(f);
        return 1;
    }

    if (fclose(f) == EOF) {
        perror("fclose");
        return 1;
    }

    return 0;
}