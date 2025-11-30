#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

#define INITIAL_CAPACITY 4

static char *read_line_stream(FILE *stream) {
    size_t cap = 128, len = 0;
    char *buf = malloc(cap);
    if (!buf) error(1, 0, "malloc failed in read_line_stream");
    int c;
    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) { free(buf); error(1, 0, "realloc failed in read_line_stream"); }
            buf = tmp;
        }
        buf[len++] = (char)c;
    }
    if (c == EOF && len == 0) { free(buf); return NULL; }
    buf[len] = '\0';
    char *out = realloc(buf, len + 1);
    if (!out) out = buf;
    return out;
}
void init_lines(char ***lines_ptr, int *size, int *capacity) {
    *capacity = INITIAL_CAPACITY;
    *size = 0;
    *lines_ptr = malloc((*capacity) * sizeof(char *));
    if (!*lines_ptr) error(1, 0, "malloc failed in init_lines");
}

void ensure_capacity(char ***lines_ptr, int *capacity, int required) {
    if (*capacity >= required) return;
    int newcap = *capacity;
    while (newcap < required) newcap *= 2;
    char **tmp = realloc(*lines_ptr, newcap * sizeof(char *));
    if (!tmp) error(1, 0, "realloc failed in ensure_capacity");
    *lines_ptr = tmp;
    *capacity = newcap;
}
void insertLine(char ***lines_ptr, int *size, int *capacity, int index, const char *text) {
    if (index < 0 || index > *size) {
        fprintf(stderr, "insertLine: invalid index %d (size=%d)\n", index, *size);
        return;
    }
    ensure_capacity(lines_ptr, capacity, (*size) + 1);
    char **lines = *lines_ptr;
    if (*size > index) {
        memmove(&lines[index + 1], &lines[index], ((*size) - index) * sizeof(char *));
    }
    size_t len = strlen(text);
    lines[index] = malloc(len + 1);
    if (!lines[index]) error(1, 0, "malloc failed in insertLine");
    memcpy(lines[index], text, len + 1);
    (*size)++;
}
void deleteLine(char ***lines_ptr, int *size, int index) {
    if (index < 0 || index >= *size) {
        fprintf(stderr, "deleteLine: invalid index %d (size=%d)\n", index, *size);
        return;
    }
    char **lines = *lines_ptr;
    free(lines[index]);
    if (index + 1 < *size) {
        memmove(&lines[index], &lines[index + 1], ((*size) - index - 1) * sizeof(char *));
    }
    (*size)--;
}
void printAllLines(char **lines, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d: %s\n", i, lines[i]);
    }
}

void shrinkToFit(char ***lines_ptr, int *capacity, int size) {
    if (size == 0) {
        free(*lines_ptr);
        *capacity = INITIAL_CAPACITY;
        *lines_ptr = malloc((*capacity) * sizeof(char *));
        if (!*lines_ptr) error(1, 0, "malloc failed in shrinkToFit (empty)");
        return;
    }
    char **tmp = realloc(*lines_ptr, size * sizeof(char *));
    if (!tmp) error(1, 0, "realloc failed in shrinkToFit");
    *lines_ptr = tmp;
    *capacity = size;
}

void saveToFile(char **lines, int size, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) error(1, 0, "fopen failed in saveToFile");
    for (int i = 0; i < size; ++i) {
        if (fprintf(f, "%s\n", lines[i]) < 0) {
            fclose(f);
            error(1, 0, "fprintf failed in saveToFile");
        }
    }
    fclose(f);
}

void loadFromFile(char ***lines_ptr, int *size, int *capacity, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "loadFromFile: cannot open '%s'\n", filename);
        return;
    }

    for (int i = 0; i < *size; ++i) free((*lines_ptr)[i]);
    *size = 0;

    char *line;
    while ((line = read_line_stream(f)) != NULL) {
        ensure_capacity(lines_ptr, capacity, (*size) + 1);
        (*lines_ptr)[(*size)++] = line;
    }
    fclose(f);
}

void freeAll(char ***lines_ptr, int *size, int *capacity) {
    for (int i = 0; i < *size; ++i) free((*lines_ptr)[i]);
    free(*lines_ptr);
    *lines_ptr = NULL;
    *size = 0;
    *capacity = 0;
}

/* Dynamic allocation of separate strings avoids wasting space:
   fixed-size 2D arrays reserve maximum space for every line; here each
   line gets exactly strlen+1 bytes and the pointer array grows only as needed. */

int main(void) {
    char **lines = NULL;
    int size = 0, capacity = 0;
    init_lines(&lines, &size, &capacity);

    while (1) {
        printf("\nCommands:\n");
        printf("1 append  2 insert  3 delete  4 print  5 shrink  6 save  7 load  8 free & exit\n");
        printf("Choice: ");
        int ch;
        if (scanf("%d", &ch) != 1) break;
        getchar();

        if (ch == 1) {
            printf("Enter line: ");
            char *ln = read_line_stream(stdin);
            if (!ln) continue;
            insertLine(&lines, &size, &capacity, size, ln);
            free(ln);
        }
        else if (ch == 2) {
            int idx;
            printf("Index: ");
            if (scanf("%d", &idx) != 1) { while (getchar() != '\n'); continue; }
            getchar();
            printf("Enter line: ");
            char *ln = read_line_stream(stdin);
            if (!ln) continue;
            insertLine(&lines, &size, &capacity, idx, ln);
            free(ln);
        }
        else if (ch == 3) {
            int idx;
            printf("Index: ");
            if (scanf("%d", &idx) != 1) { while (getchar() != '\n'); continue; }
            deleteLine(&lines, &size, idx);
        }
        else if (ch == 4) {
            printAllLines(lines, size);
        }
        else if (ch == 5) {
            shrinkToFit(&lines, &capacity, size);
            printf("Shrunk: capacity=%d size=%d\n", capacity, size);
        }
        else if (ch == 6) {
            char fname[256];
            printf("Filename: ");
            if (scanf("%255s", fname) != 1) continue;
            saveToFile(lines, size, fname);
            printf("Saved.\n");
        }
        else if (ch == 7) {
            char fname[256];
            printf("Filename: ");
            if (scanf("%255s", fname) != 1) continue;
            loadFromFile(&lines, &size, &capacity, fname);
            printf("Loaded: size=%d capacity=%d\n", size, capacity);
        }
        else if (ch == 8) {
            freeAll(&lines, &size, &capacity);
            printf("Freed and exiting.\n");
            break;
        }
        else {
            printf("Unknown command.\n");
        }
    }

    if (lines) freeAll(&lines, &size, &capacity);
    return 0;

}
