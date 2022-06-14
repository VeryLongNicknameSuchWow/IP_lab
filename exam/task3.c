#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OWNER_LEN 100

enum vv_type {
    CAR,
    BUS,
    TRUCK
};

enum d_type {
    ELECTRIC,
    HYBRID,
    COMBUSTION
};

struct Vehicle {
    char *owner;
    int d, m, y;
    enum vv_type vt;
    enum d_type dt;
    //osobowy
    int capacity;
    float enginePower;
    //autobus
    int seats;
    int stands;
    //ciężarowy
    float engineCapacity;
    float maxWeight;
};

void
init_pojazd(struct Vehicle *pvehicle, char *owner, int d, int m, int y, enum vv_type vt, enum d_type dt, void *data1,
            void *data2) {
    pvehicle->owner = owner;
    pvehicle->d = d;
    pvehicle->m = m;
    pvehicle->y = y;
    pvehicle->vt = vt;
    pvehicle->dt = dt;
    switch (vt) {
        case CAR:
            pvehicle->capacity = *((int *) data1);
            pvehicle->enginePower = *((float *) data2);
            break;
        case BUS:
            pvehicle->seats = *((int *) data1);
            pvehicle->stands = *((int *) data2);
            break;
        case TRUCK:
            pvehicle->engineCapacity = *((float *) data1);
            pvehicle->maxWeight = *((float *) data2);
            break;
    }
}

void new_owner(struct Vehicle *pvehicle) {
    char buffer[MAX_OWNER_LEN];
    fgets(buffer, MAX_OWNER_LEN, stdin);
    int len = (int) strlen(buffer);
    char *owner = malloc(len * sizeof(char));
    strcpy(owner, buffer);
    free(pvehicle->owner);
    pvehicle->owner = owner;
}

int *new_date(struct Vehicle *pvehicle, int months) {
    months += pvehicle->m;
    pvehicle->m = months % 12;
    pvehicle->y = months / 12;
}

void print_pojazd(struct Vehicle *pvehicle) {
    printf("owner: %s\n", pvehicle->owner);
    printf("date: %d-%d-%d\n", pvehicle->d, pvehicle->m, pvehicle->y);
    switch (pvehicle->vt) {
        case CAR:
            //przepraszam za Ponglish
            printf("type: car\n");
            printf("max liczba osób: %d\n", pvehicle->capacity);
            printf("moc silnika: %f\n", pvehicle->enginePower);
            break;
        case BUS:
            printf("type: bus\n");
            printf("siedzące: %d\n", pvehicle->seats);
            printf("stojące: %d\n", pvehicle->stands);
            break;
        case TRUCK:
            printf("type: truck\n");
            printf("pojemnosc silnika: %f\n", pvehicle->engineCapacity);
            printf("nacisk na os: %f\n", pvehicle->maxWeight);
            break;
    }
    switch (pvehicle->dt) {
        case ELECTRIC:
            printf("engine type: electric\n");
            break;
        case HYBRID:
            printf("engine type: hybrid\n");
            break;
        case COMBUSTION:
            printf("engine type: combustion\n");
            break;
    }
}

