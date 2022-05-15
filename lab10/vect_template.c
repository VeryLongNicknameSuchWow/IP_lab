#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 64

typedef struct Vector {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
} Vector;

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->element_size = element_size;
    vector->capacity = block_size;
    vector->size = 0;
    vector->data = malloc(block_size * element_size);
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    size_t new = vector->element_size * new_capacity;
    size_t old = vector->element_size * vector->capacity;
    if (new > old) {
        vector->data = realloc(vector->data, old * 2);
        vector->capacity *= 2;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    reserve(vector, new_size);
    if (new_size > vector->size) {
        void *destination = vector->data + vector->size * vector->element_size;
        size_t size = (new_size - vector->size) * vector->element_size;
        memset(destination, 0, size);
    }
    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    reserve(vector, vector->size + 1);
    memcpy(vector->data + vector->size * vector->element_size, value, vector->element_size);
    vector->size += 1;
}

// Remove all elements from the vector
void clear(Vector *vector) {
    vector->size = 0;
}

// Remove the last element from the vector
void pop_back(Vector *vector) {
    vector->size -= 1;
    if (vector->size < 0) {
        vector->size = 0;
    }
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value) {
    reserve(vector, vector->size + 1);
    void *source = vector->data + index * vector->element_size;
    void *destination = source + vector->element_size;
    size_t size = (vector->size - index) * vector->element_size;
    if (size > 0) {
        memmove(destination, source, size);
    }
    memcpy(source, value, vector->element_size);
    vector->size += 1;
}

// Erase element at position index
void erase(Vector *vector, int index) {
    void *destination = vector->data + index * vector->element_size;
    void *source = destination + vector->element_size;
    size_t remaining = (vector->size - index - 1) * vector->element_size;
    if (remaining > 0) {
        memmove(destination, source, remaining);
    }
    vector->size -= 1;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, int(*cmp)(const void *, const void *)) {
    int i = 0;
    while (i < vector->size) {
        void *element = vector->data + i * vector->element_size;
        if (cmp(element, value) == 0) {
            erase(vector, i);
            continue;
        }
        ++i;
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    int i = 0;
    while (i < vector->size) {
        void *element = vector->data + i * vector->element_size;
        if (predicate(element)) {
            erase(vector, i);
            continue;
        }
        ++i;
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    realloc(vector->data, vector->size * vector->element_size);
    vector->capacity = vector->size;
}

// Print integer vector
void print_vector_int(Vector *vector) {
    printf("%d\n", vector->capacity);
    for (int i = 0; i < vector->size; ++i) {
        int num = *(int *) (vector->data + vector->element_size * i);
        printf("%d ", num);
    }
}

// Print char vector
void print_vector_char(Vector *vector) {
    printf("%d\n", vector->capacity);
    for (int i = 0; i < vector->size; ++i) {
        char c = *(char *) (vector->data + vector->element_size * i);
        printf("%c ", c);
    }
}

// Print vector of Person
void print_vector_person(Vector *vector) {
    printf("%d\n", vector->capacity);
    for (int i = 0; i < vector->size; ++i) {
        Person person = *(Person *) (vector->data + vector->element_size * i);
        printf("%d ", person.age);
        for (int j = 0; j < MAX_STR_LEN; ++j) {
            if (person.first_name[j] == '\0') {
                break;
            }
            printf("%c", person.first_name[j]);
        }
        printf(" ");
        for (int j = 0; j < MAX_STR_LEN; ++j) {
            if (person.last_name[j] == '\0') {
                break;
            }
            printf("%c", person.last_name[j]);
        }
        printf("\n");
    }
}

// integer comparator - increasing order
int int_cmp(const void *v1, const void *v2) {
    int i1 = *(int *) v1;
    int i2 = *(int *) v2;
    return i1 - i2;
}

// char comparator - lexicographical order (case sensitive)
int char_cmp(const void *v1, const void *v2) {
    char c1 = *(char *) v1;
    char c2 = *(char *) v2;
    return c1 - c2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    Person pa = *(Person *) p1;
    Person pb = *(Person *) p2;
    int age_cmp = pb.age - pa.age;
    if (age_cmp != 0) {
        return age_cmp;
    }
    int fn_cmp = strcmp(pa.first_name, pb.first_name);
    if (fn_cmp != 0) {
        return fn_cmp;
    }
    int ln_cmp = strcmp(pa.last_name, pb.last_name);
    return ln_cmp;
}

// predicate: check if number is even
int is_even(void *value) {
    int number = *(int *) value;
    if (number % 2 == 0) {
        return 1;
    }
    return 0;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char c = *(char *) value;
    const char vowels[] = {'a', 'e', 'i', 'o', 'u', 'y',
                           'A', 'E', 'I', 'O', 'U', 'Y'};
    for (int i = 0; i < 12; ++i) {
        if (c == vowels[i]) {
            return 1;
        }
    }
    return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person p = *(Person *) person;
    return p.age > 25;
}

// -------------------------------------------------------------

void read_int(void *value) {
    scanf("%d", (int *) value);
}

void read_char(void *value) {
    char c[2];
    scanf("%s", c);
    *(char *) value = c[0];
}

void read_person(void *value) {
    Person *person = (Person *) value;
    scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void(*read)(void *), int (*cmp)(const void *, const void *), int(*predicate)(void *)) {
    char op[2];
    int index;
    size_t size;
    void *v = malloc(vector->element_size);
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'p': // push_back
                read(v);
                push_back(vector, v);
                break;
            case 'i': // insert
                scanf("%d", &index);
                read(v);
                insert(vector, index, v);
                break;
            case 'e': // erase
                scanf("%d", &index);
                read(v);
                erase(vector, index);
                erase_value(vector, v, cmp);
                break;
            case 'd': // erase (predicate)
                erase_if(vector, predicate);
                break;
            case 'r': // resize
                scanf("%zu", &size);
                resize(vector, size);
                break;
            case 'c': // clear
                clear(vector);
                break;
            case 'f': // shrink
                shrink_to_fit(vector);
                break;
            case 's': // sort
                qsort(vector->data, vector->size,
                      vector->element_size, cmp);
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
    free(v);
}

int main(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);

    switch (to_do) {
        case 1:
            init_vector(&vector_int, 4, sizeof(int));
            vector_test(&vector_int, n, read_int, int_cmp, is_even);
            print_vector_int(&vector_int);
            free(vector_int.data);
            break;
        case 2:
            init_vector(&vector_char, 2, sizeof(char));
            vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
            print_vector_char(&vector_char);
            free(vector_char.data);
            break;
        case 3:
            init_vector(&vector_person, 2, sizeof(Person));
            vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
            print_vector_person(&vector_person);
            free(vector_person.data);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }

    return 0;
}

