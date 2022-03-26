#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)
#define MAX_LINE 128

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = {0};

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp(const void *a, const void *b) {
    int va = *(int *) a;
    int vb = *(int *) b;
    if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
    return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di(const void *a, const void *b) {
    int va = *(int *) a;
    int vb = *(int *) b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin
void wc(int *nl, int *nw, int *nc) {
    char line[MAX_LINE];
    *nl = 0;
    *nw = 0;
    *nc = 0;
    while (1) {
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }
        *nl += 1;

        bool foundWordStart = false;
        for (int i = 0; i < MAX_LINE; ++i) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
                foundWordStart = true;
                continue;
            }
            if (foundWordStart) {
                if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
                    *nw += 1;
                    foundWordStart = false;
                }
            }
            if (line[i] == '\n') {
                break;
            }
        }

        for (int i = 0; i < MAX_LINE; ++i) {
            if (line[i] != '\n') {
                *nc += 1;
            } else {
                *nc += 1;
                break;
            }
        }
    }
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt) {
    char line[MAX_LINE];

    int charCount[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; ++i) {
        charCount[i] = 0;
    }

    char chars[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; ++i) {
        chars[i] = (char) (i + FIRST_CHAR);
    }

    while (1) {
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }

        for (int i = 0; i < MAX_LINE; ++i) {
            if (line[i] == '\n') {
                break;
            }
            int charInt = (int) line[i];
            if (charInt >= FIRST_CHAR && charInt < LAST_CHAR) {
                charCount[charInt - FIRST_CHAR] += 1;
                chars[charInt - FIRST_CHAR] = line[i];
            }
        }
    }

    bool swapped = false;
    do {
        swapped = false;
        for (int i = 1; i < MAX_CHARS; ++i) {
            if (charCount[i - 1] < charCount[i]) {
                char temp_c = chars[i];
                chars[i] = chars[i - 1];
                chars[i - 1] = temp_c;

                int tempCount = charCount[i];
                charCount[i] = charCount[i - 1];
                charCount[i - 1] = tempCount;

                swapped = true;
            }
        }
    } while (swapped);

    *n_char = (int) chars[char_no - 1];
    *cnt = charCount[char_no - 1];
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[]) {
    char line[MAX_LINE];
    int chars[MAX_DIGRAMS] = {0};

    for (int i = 0; i < MAX_DIGRAMS; ++i) {
        chars[i] = i;
    }

    while (1) {
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }

        for (int i = 1; i < MAX_LINE; ++i) {
            if (line[i - 1] == '\n' || line[i] == '\n') {
                break;
            }
            char a = line[i - 1];
            char b = line[i];
            if (a >= FIRST_CHAR && a < LAST_CHAR && b >= FIRST_CHAR && b < LAST_CHAR) {
                count[(a - FIRST_CHAR) * MAX_CHARS + (b - FIRST_CHAR)] += 1;
            }
        }
    }

    qsort(chars, MAX_DIGRAMS, sizeof(int), cmp_di);

    digram[0] = chars[digram_no - 1] / MAX_CHARS + FIRST_CHAR;
    digram[1] = chars[digram_no - 1] % MAX_CHARS + FIRST_CHAR;
    digram[2] = count[chars[digram_no - 1]];
}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter) {
    char line[MAX_LINE];
    *line_comment_counter = 0;
    *block_comment_counter = 0;
    bool inBlockComment = false;
    while (1) {
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }

        for (int i = 1; i < MAX_LINE; ++i) {
            if (line[i - 1] == '\n' || line[i] == '\n') {
                break;
            }
            if (!inBlockComment) {
                if (line[i - 1] == '/' && line[i] == '/') {
                    *line_comment_counter += 1;
                    break;
                }
                if (line[i - 1] == '/' && line[i] == '*') {
                    inBlockComment = true;
                    i += 1;
                    continue;
                }
            } else {
                if (line[i - 1] == '*' && line[i] == '/') {
                    inBlockComment = false;
                    *block_comment_counter += 1;
                    i += 1;
                    continue;
                }
            }
        }
    }
}

int read_line() {
    char line[MAX_LINE];
    int n;

    fgets(line, MAX_LINE, stdin); // to get the whole line
    sscanf(line, "%d", &n);
    return n;
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int digram[3];

    to_do = read_line();
    switch (to_do) {
        case 1: // wc()
            wc(&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_line();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // digram_count()
            char_no = read_line();
            digram_count(char_no, digram);
            printf("%c%c %d\n", digram[0], digram[1], digram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
