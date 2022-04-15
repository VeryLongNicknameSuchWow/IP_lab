#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair *, int, pair);

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *, int);

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair *, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *, int);

int find_max_elements(pair *, int, int *);

int find_min_elements(pair *, int, int *);

int get_domain(pair *, int, int *);

// Case 3:
int composition(pair *, int, pair *, int, pair *);

// pair struct comparator
int cmp(pair, pair);

int add_relation(pair *relation, int size, pair pair) {
    int insertAt = 0;
    for (int i = 0; i < size; ++i) {
        int comparison = cmp(relation[i], pair);
        if (comparison == 0) {
            return size;
        } else if (comparison < 0) {
            insertAt++;
        } else {
            break;
        }
    }
    for (int i = size - 1; i >= insertAt; --i) {
        relation[i + 1] = relation[i];
    }
    relation[insertAt] = pair;
    return size + 1;
}

int is_reflexive(pair *relation, int size) {
    int domain[MAX_REL_SIZE];
    int domain_size = get_domain(relation, size, domain);
    for (int i = 0; i < domain_size; ++i) {
        int found = 0;
        for (int j = 0; j < size; ++j) {
            if (relation[j].first == domain[i] && relation[j].second == domain[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}

int is_irreflexive(pair *relation, int size) {
    for (int i = 0; i < size; ++i) {
        if (relation[i].first == relation[i].second) {
            return 0;
        }
    }
    return 1;
}

int is_symmetric(pair *relation, int size) {
    for (int i = 0; i < size; ++i) {
        int found = 0;
        for (int j = 0; j < size; ++j) {
            if (relation[i].first == relation[j].second && relation[j].first == relation[i].second) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}

int is_antisymmetric(pair *relation, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (relation[i].first == relation[j].second && relation[j].first == relation[i].second) {
                if (relation[i].first != relation[i].second) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_asymmetric(pair *relation, int size) {
    return is_antisymmetric(relation, size) && is_irreflexive(relation, size);
}

int is_transitive(pair *relation, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (relation[i].second != relation[j].first) {
                continue;
            }
            int found = 0;
            for (int k = 0; k < size; ++k) {
                if (relation[k].first == relation[i].first && relation[k].second == relation[j].second) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return 0;
            }
        }
    }
    return 1;
}

int is_partial_order(pair *relation, int size) {
    return is_transitive(relation, size) && is_antisymmetric(relation, size) && is_reflexive(relation, size);
}

int is_total_order(pair *relation, int size) {
    return is_transitive(relation, size) && is_antisymmetric(relation, size) && is_connected(relation, size) && is_reflexive(relation, size);
}

int is_connected(pair *relation, int size) {
    int domain[MAX_REL_SIZE];
    int domain_size = get_domain(relation, size, domain);
    for (int i = 0; i < domain_size; ++i) {
        for (int j = 0; j < domain_size; ++j) {
            int found = 0;
            for (int k = 0; k < size; ++k) {
                if (relation[k].first == domain[i] && relation[k].second == domain[j]) {
                    found = 1;
                    break;
                }
                if (relation[k].second == domain[i] && relation[k].first == domain[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return 0;
            }
        }
    }
    return 1;
}

int find_max_elements(pair *relation, int size, int *max_elements) {
    int domain[MAX_REL_SIZE];
    int domain_size = get_domain(relation, size, domain);
    int max_elements_size = 0;

    for (int i = 0; i < domain_size; ++i) {
        int is_max = 1;
        int num = domain[i];
        for (int j = 0; j < size; ++j) {
            if (relation[j].first == num && relation[j].second != num) {
                is_max = 0;
                break;
            }
        }
        if (is_max) {
            int insertAt = 0;
            int found = 0;
            for (int j = 0; j < max_elements_size; ++j) {
                if (max_elements[j] == num) {
                    found = 1;
                    break;
                } else if (max_elements[j] < num) {
                    insertAt++;
                } else {
                    break;
                }
            }
            if (found) {
                continue;
            }
            for (int j = max_elements_size - 1; j >= insertAt; --j) {
                max_elements[j + 1] = max_elements[j];
            }
            max_elements[insertAt] = num;
            max_elements_size++;
        }
    }
    return max_elements_size;
}

int find_min_elements(pair *relation, int size, int *max_elements) {
    int domain[MAX_REL_SIZE];
    int domain_size = get_domain(relation, size, domain);
    int max_elements_size = 0;

    for (int i = 0; i < domain_size; ++i) {
        int is_max = 1;
        int num = domain[i];
        for (int j = 0; j < size; ++j) {
            if (relation[j].first != num && relation[j].second == num) {
                is_max = 0;
                break;
            }
        }
        if (is_max) {
            int insertAt = 0;
            int found = 0;
            for (int j = 0; j < max_elements_size; ++j) {
                if (max_elements[j] == num) {
                    found = 1;
                    break;
                } else if (max_elements[j] < num) {
                    insertAt++;
                } else {
                    break;
                }
            }
            if (found) {
                continue;
            }
            for (int j = max_elements_size - 1; j >= insertAt; --j) {
                max_elements[j + 1] = max_elements[j];
            }
            max_elements[insertAt] = num;
            max_elements_size++;
        }
    }
    return max_elements_size;
}

int get_domain(pair *relation, int size, int *domain) {
    int domain_size = 0;

    for (int i = 0; i < size; ++i) {
        int x[2] = {relation[i].first, relation[i].second};
        for (int j = 0; j < 2; ++j) {
            int num = x[j];
            int insertAt = 0;
            int found = 0;
            for (int k = 0; k < domain_size; ++k) {
                if (domain[k] == num) {
                    found = 1;
                    break;
                } else if (domain[k] < num) {
                    insertAt++;
                } else {
                    break;
                }
            }
            if (found) {
                continue;
            }
            for (int k = size - 1; k >= insertAt; --k) {
                domain[k + 1] = domain[k];
            }
            domain[insertAt] = num;
            domain_size++;
        }
    }

    return domain_size;
}

int composition(pair *relation_1, int size_1, pair *relation_2, int size_2, pair *comp_relation) {
    int comp_size = 0;
    for (int i = 0; i < size_1; ++i) {
        for (int j = 0; j < size_2; ++j) {
            if (relation_1[i].second == relation_2[j].first) {
                pair pair = {relation_1[i].first, relation_2[j].second};
                comp_size = add_relation(comp_relation, comp_size, pair);
            }
        }
    }
    return comp_size;
}

int cmp(pair p1, pair p2) {
    if (p1.first == p2.first) {
        return p1.second - p2.second;
    }
    return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n, first, second;
    int size = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &first, &second);
        pair pair = {first, second};
        size = add_relation(relation, size, pair);
    }
    return size;
}

void print_int_array(int *array, int n) {
    printf("%d\n", n);
    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];

    scanf("%d", &to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) {
                break;
            }
            int max_elements[MAX_REL_SIZE];
            int min_elements[MAX_REL_SIZE];
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

