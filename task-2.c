#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "w");
    if (!f) {
        perror("fopen");
        return 1;
    }

    printf("Enter your text. To finish, press Ctrl-F.\n");

    int ch;
    while ((ch = getc(stdin)) != EOF) {
        if (ch == '#') {
            break;
        }
        if (putc(ch, f) == EOF) {
            perror("putc");
            fclose(f);
            return 1;
        }
    }

    if (fclose(f) == EOF) {
        perror("fclose");
        return 1;
    }

    return 0;
}
