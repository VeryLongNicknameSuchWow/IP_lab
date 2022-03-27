#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024
#define MIN_B_CHAR 65
#define MAX_B_CHAR 91 // exclusive
#define MIN_S_CHAR 97
#define MAX_S_CHAR 123 // EXCLUSIVE
#define FLOOR 95 // exact
#define MIN_NUMBER 48
#define MAX_NUMBER 58
#define KEYWORDS_NUM 32


int mcmp(const void *, const void *);

char tab[MAX_IDS][MAX_ID_LEN];

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

int valid_char(int c) {
    if ((c >= MIN_B_CHAR && c < MAX_B_CHAR) || (c >= MIN_S_CHAR && c < MAX_S_CHAR) || (c == FLOOR) ||
        (c >= MIN_NUMBER && c < MAX_NUMBER)) {
        return 1;
    } else {
        return 0;
    }
}

int valid_fchar(int c) {
    if ((c >= MIN_B_CHAR && c < MAX_B_CHAR) || (c >= MIN_S_CHAR && c < MAX_S_CHAR) || (c == FLOOR)) {
        return 1;
    }
    return 0;
}

int find_idents() {
    int buffer, counter = 0, sub_c, added = 1;

    while (1) {
        if (added) {
            buffer = getchar();
        }
        added = 1;
        if (buffer == EOF) {
            break;
        }
        // skipping comments



        // at this point buffer is non EOF char
        if (buffer == '/') {
            // a code for detecting what comment will be checked
            buffer = getchar();
            if (buffer == '/') {
                // its a line comment
                // goes through chars until end of line
                while (1) {
                    buffer = getchar();
                    if (buffer == 10 || buffer == EOF) {
                        break;
                    }
                }
            } else if (buffer == '*') {
                // its a block comment
                // goes through chars until enf of comment
                buffer = getchar();
                while (1) {
                    if (buffer == '*' || buffer == EOF) {
                        buffer = getchar();
                        if (buffer == '/' || buffer == EOF) {
                            break;
                        }
                    }
                    if (buffer == '\\') {
                        buffer = getchar();
                    }
                    buffer = getchar();
                }
            }
        }
        // end of skipping comments

        // skipping pre processor commands
        if (buffer == 35) {
            buffer = getchar();
            if (buffer == 'i') {
                buffer = getchar();
                if (buffer == 'n') {
                    while (1) {
                        buffer = getchar();
                        if (buffer == 10 || buffer == EOF) {
                            break;
                        }
                    }
                } else {
                    while (buffer != 32) {
                        buffer = getchar();
                        if (buffer == EOF) {
                            break;
                        }
                    }
                }
            } else {
                while (buffer != 32) {
                    buffer = getchar();
                    if (buffer == EOF) {
                        break;
                    }
                }
            }
        }
        // end of skipping librarys


        // skipping commas
        if (buffer == 34) {
            buffer = getchar();
            while (buffer != 34) {
                if (buffer == '\\') {
                    buffer = getchar();
                }
                if (buffer == EOF) {
                    break;
                }
                buffer = getchar();
            }
        }

        if (buffer == 39) {
            buffer = getchar();
            while (buffer != 39) {
                if (buffer == '\\') {
                    buffer = getchar();
                }
                if (buffer == EOF) {
                    break;
                }
                buffer = getchar();
            }
        }
        // end of skipping commas

        // checks if an identifier starts
        if (valid_fchar(buffer)) {
            added = 0;
            sub_c = 0;
            // the buffer is a start of an identifier or a keyword
            while (valid_char(buffer)) {
                tab[counter][sub_c] = (char) buffer;
                sub_c++;
                buffer = getchar();
                if (buffer == EOF) {
                    break;
                }
            }
            counter++;
        } else if (valid_char(buffer)) {
            added = 0;
            buffer = getchar();
            while (valid_char(buffer)) {
                buffer = getchar();
                if (buffer == EOF) {
                    break;
                }
            }
        }
        if (buffer == EOF) {
            break;
        }
    }

    qsort(tab, counter, sizeof(char) * MAX_ID_LEN, mcmp);

    // counting identifiers
    int ans = 0, flag;

    for (int i = 0; i < counter; i++) {
        flag = 1;
        for (int j = 0; j < KEYWORDS_NUM; j++) {
            if (!(strcmp(keywords[j], tab[i]))) {
                flag = 0;
            }
            if (i > 0) {
                if (!(abs((int) strcmp(tab[i], tab[i - 1])))) {
                    flag = 0;
                }
            }
        }
        if (flag) {
            ans++;
        }
    }

    return ans;
}

int mcmp(const void *first_arg, const void *second_arg) {
    return strcmp(first_arg, second_arg);
}

int main(void) {
    printf("%d\n", find_idents());
    return 0;
}