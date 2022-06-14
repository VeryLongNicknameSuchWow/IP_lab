#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data {
    int tab[20];
    int in_tab;   // liczba aktywnych elementów w tablicy tab
    int sum;      // suma aktywnych elementów
};

struct ListElement {
    struct Data data;
    struct ListElement *prev;
    struct ListElement *next;
};

struct List {
    struct ListElement *head;
    struct ListElement *tail;
};

struct ListElement *createListElement(const struct Data *data) {
    struct ListElement *element = malloc(sizeof(struct ListElement));
    element->data = *data;
    element->prev = NULL;
    element->next = NULL;
    return element;
}

void initList(struct List *plist) {
    plist = malloc(sizeof(struct List));
    plist->head = NULL;
    plist->tail = NULL;
}

void addElement(struct List *plist, const struct Data *data) {
    struct ListElement *element = createListElement(data);
    if (plist->head == NULL) {
        plist->head = element;
        plist->tail = element;
        return;
    }
    plist->tail->next = element;
    element->prev = plist->tail;
    plist->tail = element;
}

void print_above(const struct List *plist, int threshold) {
    struct ListElement *current = plist->head;
    while (current != NULL) {
        struct Data data = current->data;
        if (data.in_tab > threshold) {
            for (int i = 0; i < data.in_tab; ++i) {
                printf("%d ", data.tab[i]);
            }
        }
        current = current->next;
    }
}

void print_if(const struct List *plist, int (*pred)(const struct Data *data)) {
    struct ListElement *current = plist->head;
    while (current != NULL) {
        struct Data data = current->data;
        if (pred(&data)) {
            for (int i = 0; i < data.in_tab; ++i) {
                printf("%d ", data.tab[i]);
            }
        }
        current = current->next;
    }
}

int pred(const struct Data *data) {
    return data->sum % 2 == 0;
}

void test_print_if(const struct List *plist) {
    print_if(plist, pred);
}