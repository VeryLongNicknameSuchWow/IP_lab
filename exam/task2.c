#include <stdlib.h>
#include <string.h>

typedef struct tagData {
    int *numbers;
    int size;
} Data;

typedef struct tagStackElement {
    Data data;
    struct tagStackElement *next; // powinien być domyślnie NULLem
} StackElement;

typedef struct tagStack {
    StackElement *head; // nowe elementy stosu są dodawane na początku
} Stack;

void push(Stack *plist, const Data *pdata) {
    StackElement *element = malloc(sizeof(StackElement));
    element->data = *pdata;
    element->next = plist->head;
    plist->head = element;
}

Data peek(const Stack *plist) {
    if (plist->head == NULL) {
        Data result;
        result.numbers = NULL;
        return result;
    }
    return plist->head->data;
}

int pop(Stack *plist, Data *pdata) {
    if (plist->head == NULL) {
        return -1;
    }
    memcpy(pdata, &plist->head->data, sizeof(Data));
    StackElement *clear = plist->head;
    plist->head = plist->head->next;
    free(clear);
    if (plist->head == NULL) {
        return 0;
    }
    return 1;
}

int free_stack(Stack *pstack, int **table) {
    int i = 0;
    Data data;
    while (pop(pstack, &data) != -1) {
        table[i++] = data.numbers;
    }
    return i;
}

