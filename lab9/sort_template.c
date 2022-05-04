#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2) {
    Person pa = *(Person *) p1;
    Person pb = *(Person *) p2;
    if (pa.age == pb.age) {
        int first_name_comparison = strcmp(pa.first_name, pb.first_name);
        if (first_name_comparison == 0) {
            return strcmp(pa.last_name, pb.last_name);
        }
        return first_name_comparison;
    }
    return pb.age - pa.age;
}

// Read data to Person array (till EOF)
int read_person_array(Person *persons) {
    char buffer[MAX_STR_LEN];

    int i = 0;
    while (1) {
        if (fgets(buffer, MAX_STR_LEN, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // remove \n character at the end of the line
        if (buffer[0] == '\0') {
            continue;
        }

        Person person;
        char delim[] = " ";
        char *age = strtok(buffer, delim);
        char *first_name = strtok(NULL, delim);
        char *last_name = strtok(NULL, delim);
        person.age = (int) strtol(age, NULL, 10);
        strcpy(person.first_name, first_name);
        strcpy(person.last_name, last_name);

        persons[i++] = person;
    }
    return i;
}

// Print Person array
void print_person_array(Person *persons, int n) {
    for (int i = 0; i < n; ++i) {
        Person person = persons[i];
        printf("%d ", person.age);
        for (int j = 0; j < MAX_STR_LEN; ++j) {
            char c = person.first_name[j];
            if (c == '\0') {
                break;
            }
            printf("%c", c);
        }
        printf(" ");
        for (int j = 0; j < MAX_STR_LEN; ++j) {
            char c = person.last_name[j];
            if (c == '\0') {
                break;
            }
            printf("%c", c);
        }
        printf("\n");
    }
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2) {
    char *la_ptr = (char *) l1;
    char *lb_ptr = (char *) l2;
    char la[MAX_STR_LEN];
    char lb[MAX_STR_LEN];
    strcpy(la, la_ptr);
    strcpy(lb, lb_ptr);

    char delim[] = " ";
    char *age_a_ptr = strtok(la, delim);
    int age_a = strtol(age_a_ptr, NULL, 10);
    char *first_name_a = strtok(NULL, delim);
    int woman_a = first_name_a[strlen(first_name_a) - 1] == 'a';
    char *last_name_a = strtok(NULL, delim);

    char *age_b_ptr = strtok(lb, delim);
    int age_b = strtol(age_b_ptr, NULL, 10);
    char *first_name_b = strtok(NULL, delim);
    int woman_b = first_name_b[strlen(first_name_b) - 1] == 'a';
    char *last_name_b = strtok(NULL, delim);

    // sort women first
    if (woman_a != woman_b) {
        return woman_b - woman_a;
    }

    // both women, sort by age
    if (woman_a) {
        return age_a - age_b;
    }

    // both men, sort by last name
    return strcmp(last_name_a, last_name_b);
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]) {
    char buffer[MAX_STR_LEN];

    int i = 0;
    while (1) {
        if (fgets(buffer, MAX_STR_LEN, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // remove \n character at the end of the line
        if (buffer[0] == '\0') {
            continue;
        }
        strcpy(lines[i++], buffer);
    }
    return i;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < MAX_STR_LEN; ++j) {
            char c = lines[i][j];
            if (c == '\0') {
                break;
            }
            printf("%c", c);
        }
        printf("\n");
    }
}

// -------------------------------------------------

int read_int() {
    char buf[MAX_STR_LEN];
    int n;
    fgets(buf, MAX_STR_LEN, stdin);
    sscanf(buf, "%d", &n);
    return n;
}

int main(void) {
    int to_do = read_int();
    int n;
    Person persons[MAX_PERSONS];
    char lines[MAX_PERSONS][MAX_STR_LEN];
    switch (to_do) {
        case 1:
            n = read_person_array(persons);
            qsort(persons, (size_t) n, sizeof(Person), cmp_person);
            print_person_array(persons, n);
            break;
        case 2:
            n = read_lines(lines);
            qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
            print_lines(lines, n);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
}

