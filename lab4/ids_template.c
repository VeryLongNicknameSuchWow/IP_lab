#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum State {
    NONE,
    IN_LINE_COMMENT,
    IN_BLOCK_COMMENT,
    START_BLOCK_COMMENT,
    END_BLOCK_COMMENT,
    IN_STRING,
    IN_CHAR,
    IN_ID,
    IN_INCLUDE
} State;

#define MAX_ID_LEN 64
#define MAX_IDS 1024
#define MAX_LINE 128

int index_cmp(const void *, const void *);

int cmp(const void *, const void *);

char tab[MAX_IDS][MAX_ID_LEN];
int currentID = 0;
int lineCounter = 0;

char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};

bool isKeyword(char id[]) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        if (strcmp(id, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

void add_ID(char id[]) {
    if (id[0] == '_') {
        printf("%d", lineCounter);
    }
    if (!isKeyword(id)) {
        strcpy(tab[currentID++], id);
    }
}

int find_idents() {
    char line[MAX_LINE];
    enum State state = NONE;

    while (1) {
        lineCounter += 1;

        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }
        if (strncmp("#include", line, 8) == 0) {
            goto continue_line_loop;
        }
        if (strncmp("#define", line, 7) == 0) {
            goto continue_line_loop;
        }

        char id[MAX_ID_LEN] = {'\0'};
        int id_char_i = 0;

        for (int i = 0; i < MAX_LINE; ++i) {
            char c = line[i];

            if (i == 0 && state == IN_CHAR) {
                state = NONE;
            }

            switch (state) {
                case NONE:
                    if (isalpha(c) || c == '_') {
                        state = IN_ID;
                        id[id_char_i++] = c;
                        goto continue_char_loop;
                    }
                    if (c == '/') {
                        if (line[i + 1] == '/') {
                            state = NONE;
                            goto continue_line_loop;
                        }
                        if (line[i + 1] == '*') {
                            state = START_BLOCK_COMMENT;
                            goto continue_char_loop;
                        }
                    } else if (c == '"') {
                        state = IN_STRING;
                        goto continue_char_loop;
                    } else if (c == '\'') {
                        state = IN_CHAR;
                        goto continue_char_loop;
                    }
                    if (c == '\n') {
                        goto continue_line_loop;
                    }
                    goto continue_char_loop;
                case IN_BLOCK_COMMENT:
                    if (c == '*' && line[i + 1] == '/') {
                        state = END_BLOCK_COMMENT;
                        goto continue_char_loop;
                    } else if (c == '\n') {
                        goto continue_line_loop;
                    }
                    goto continue_char_loop;
                case START_BLOCK_COMMENT:
                    state = IN_BLOCK_COMMENT;
                    goto continue_char_loop;
                case END_BLOCK_COMMENT:
                    state = NONE;
                    goto continue_char_loop;
                case IN_STRING:
                    if (c == '"' && line[i - 1] != '\\' && line[i - 2] != '\\') {
                        state = NONE;
                        goto continue_char_loop;
                    }
                    goto continue_char_loop;
                case IN_CHAR:
                    if (c == '\'' && line[i - 1] != '\\' && line[i - 2] != '\\') {
                        state = NONE;
                        goto continue_char_loop;
                    }
                    goto continue_char_loop;
                case IN_ID:
                    if (isalnum(c) || c == '_') {
                        id[id_char_i++] = c;
                    } else if (c == '\n') {
                        state = NONE;
                        add_ID(id);
                        for (int j = 0; j < MAX_ID_LEN; ++j) {
                            id[j] = '\0';
                        }
                        goto continue_line_loop;
                    } else {
                        state = NONE;
                        add_ID(id);
                        for (int j = 0; j < MAX_ID_LEN; ++j) {
                            id[j] = '\0';
                        }
                        id_char_i = 0;
                        goto continue_char_loop;
                    }
                    goto continue_char_loop;
            }
            continue_char_loop:;
        }
        continue_line_loop:;
    }

    return currentID;
}

int cmp(const void *first_arg, const void *second_arg) {
    char *a = *(char **) first_arg;
    char *b = *(char **) second_arg;
    return strcmp(a, b);
}

int index_cmp(const void *first_arg, const void *second_arg) {
    int a = *(int *) first_arg;
    int b = *(int *) second_arg;
    return strcmp(tab[a], tab[b]);
}

int main(void) {
    find_idents();

    char temp[MAX_ID_LEN];
    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < currentID - 1; ++i) {
            int compare = strcmp(tab[i], tab[i + 1]);
            if (compare > 0) {
                memset(temp, '\0', sizeof(char) * MAX_ID_LEN);
                strcpy(temp, tab[i]);
                memset(tab[i], '\0', sizeof(char) * MAX_ID_LEN);
                strcpy(tab[i], tab[i + 1]);
                memset(tab[i + 1], '\0', sizeof(char) * MAX_ID_LEN);
                strcpy(tab[i + 1], temp);
                done = false;
            } else if (compare == 0) {
                memset(tab[i + 1], '\0', sizeof(char) * MAX_ID_LEN);
                strcpy(tab[i + 1], tab[--currentID]);
                done = false;
                break;
            }
        }
    }


    for (int id = 0; id < currentID; ++id) {
        bool printed = false;
        for (int charI = 0; charI < MAX_ID_LEN; ++charI) {
            char c = tab[id][charI];
            if (c != '\0') {
                printf("%c", c);
                printed = true;
            }
        }
        if (printed) {
            printf("\n");
        }
    }

    printf("%d\n", currentID);
    return 0;
}

