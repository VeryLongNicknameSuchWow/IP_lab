#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

struct tagList;

typedef void (*ConstDataFp)(const void *);

typedef void (*DataFp)(void *);

typedef int (*CompareDataFp)(const void *, const void *);

typedef void (*InsertInOrder)(struct tagList *, void *);

typedef struct tagListElement {
    struct tagListElement *next;
    void *data;
} ListElement;

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, InsertInOrder insert_sorted) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->insert_sorted = insert_sorted;
}

// Print all elements of the list
void dump_list(const List *p_list) {
    ListElement *current = p_list->head;
    while (current != NULL) {
        p_list->dump_data(current->data);
        current = current->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement *current = p_list->head;
    while (current != NULL) {
        if (p_list->compare_data(current->data, data) == 0) {
            p_list->dump_data(current->data);
        }
        current = current->next;
    }
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data) {
    ListElement *element = malloc(sizeof(ListElement));
    if (element == NULL) {
        exit(MEMORY_ALLOCATION_ERROR);
    }
    element->data = data;
    element->next = p_list->head;
    if (p_list->tail == NULL) {
        p_list->tail = element;
    }
    p_list->head = element;
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    ListElement *element = malloc(sizeof(ListElement));
    if (element == NULL) {
        exit(MEMORY_ALLOCATION_ERROR);
    }
    element->data = data;
    element->next = NULL;
    if (p_list->tail != NULL) {
        p_list->tail->next = element;
    }
    p_list->tail = element;
    if (p_list->head == NULL) {
        p_list->head = element;
    }
}

// Remove the first element
void pop_front(List *p_list) {
    if (p_list->head == NULL) {
        return;
    }
    ListElement *delete = p_list->head;
    p_list->head = p_list->head->next;
    if (p_list->head == NULL) {
        p_list->tail = NULL;
    }
    p_list->free_data(delete->data);
    free(delete);
}

// Free all elements of the list
void free_list(List *p_list) {
    ListElement *curr = p_list->head;
    while (curr != NULL) {
        ListElement *delete = curr;
        curr = curr->next;
        if (p_list->free_data != NULL) {
            p_list->free_data(delete->data);
        }
        free(delete);
    }
    p_list->head = NULL;
    p_list->tail = NULL;
}

// Reverse the list
void reverse(List *p_list) {
    ListElement *prev = NULL;
    ListElement *curr = p_list->head;
    ListElement *tmp;
    while (curr != NULL) {
        tmp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = tmp;
    }
    tmp = p_list->head;
    p_list->head = p_list->tail;
    p_list->tail = tmp;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data) {
    p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement *find_insertion_point(const List *p_list, void *data) {
    ListElement *insertion_point = NULL;
    ListElement *current = p_list->head;
    while (current != NULL) {
        if (p_list->compare_data(current->data, data) >= 0) {
            insertion_point = current;
        } else {
            break;
        }
        current = current->next;
    }
    return insertion_point;
}

// Insert element after 'previous'
void push_after(List *p_list, ListElement *element, ListElement *previous) {
    element->next = previous->next;
    previous->next = element;
    if (previous == p_list->tail) {
        p_list->tail = element;
    }
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data) {
    ListElement *element = malloc(sizeof(ListElement));
    if (element == NULL) {
        exit(MEMORY_ALLOCATION_ERROR);
    }
    element->data = p_data;
    element->next = NULL;
    if (p_list->head == NULL) {
        p_list->head = element;
        p_list->tail = element;
        return;
    }
    if (p_list->compare_data(p_list->head->data, p_data) < 0) {
        element->next = p_list->head;
        p_list->head = element;
        return;
    }

    ListElement *previous = find_insertion_point(p_list, p_data);
    if (p_list->compare_data(previous->data, p_data) == 0) {
        return;
    }
    push_after(p_list, element, previous);
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt {
    int id;
} DataInt;

DataInt *create_data_int(int v) {
    DataInt *dataInt = malloc(sizeof(DataInt));
    if (dataInt == NULL) {
        exit(MEMORY_ALLOCATION_ERROR);
    }
    dataInt->id = v;
    return dataInt;
}

void dump_int(const void *d) {
    DataInt *dataInt = (DataInt *) d;
    printf("%d ", dataInt->id);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    int A = *(int *) a;
    int B = *(int *) b;
    return B - A;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

DataWord *create_data_word(char *word, int counter) {
    DataWord *dataWord = malloc(sizeof(DataWord));
    if (dataWord == NULL) {
        exit(MEMORY_ALLOCATION_ERROR);
    }
    dataWord->word = strdup(word);
    dataWord->counter = counter;
    return dataWord;
}

void dump_word(const void *d) {
    DataWord *dataWord = (DataWord *) d;
    printf("%s", dataWord->word);
    printf("\n");
}

void dump_word_lowercase(const void *d) {
    DataWord *dataWord = (DataWord *) d;
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        char c = dataWord->word[i];
        if (c == '\0') {
            break;
        }
        printf("%c", tolower(c));
    }
    printf("\n");
}

void free_word(void *d) {
    DataWord *dataWord = (DataWord *) d;
    free(dataWord->word);
    free(dataWord);
}

// conpare words case insensitive
int cmp_word_alphabet(const void *a, const void *b) {
    DataWord *dataWordA = (DataWord *) a;
    DataWord *dataWordB = (DataWord *) b;
    return strcasecmp(dataWordA->word, dataWordB->word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord *dataWordA = (DataWord *) a;
    DataWord *dataWordB = (DataWord *) b;
    return dataWordB->counter - dataWordA->counter;
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data) {
    ListElement *current = p_list->head;
    while (current != NULL) {
        if (cmp_word_alphabet(current->data, data) == 0) {
            DataWord *dataWord = (DataWord *) current->data;
            dataWord->counter++;
            return;
        }
        current = current->next;
    }

    insert_elem(p_list, data);
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order) {
    char buffer[BUFFER_SIZE];
    const char delim[] = {' ', '\t', '\n', '\r', '.', ',', '?', '!', ':', ';', '-', '\0'};
    while (1) {
        if (fgets(buffer, BUFFER_SIZE, stream) == NULL) {
            break;
        }
        char *word = strtok(buffer, delim);
        while (word != NULL) {
            if (order == 0) {
                push_back(p_list, create_data_word(word, 1));
            } else {
                insert_elem_counter(p_list, create_data_word(word, 1));
            }
            word = strtok(NULL, delim);
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
    char op[2];
    int v;
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d", &n);
            init_list(&list, dump_int, free_int,
                      cmp_int, insert_elem);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 0);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d", &n);
            init_list(&list, dump_word_lowercase, free_word,
                      cmp_word_alphabet, insert_elem_counter);
            stream_to_list(&list, stdin, 1);
            list.compare_data = cmp_word_counter;
            DataWord data = {NULL, n};
//			list.dump_data = dump_word_lowercase;
            reverse(&list);
            dump_list_if(&list, &data);
            printf("\n");
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

