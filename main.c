#include <stdio.h>
#include <stdlib.h>

typedef struct bucket {
    int IsEmpty;
    int value;
} bucket;

typedef struct hash_set {
    bucket* array;
    int max_size;
} hash_set;

int hash_function(hash_set* H, int key);
int size(hash_set* H);
float load_factor(hash_set* H);
int IsFull(hash_set* H);
hash_set* add(hash_set* H, int entry);
hash_set* CreateEmpty();
int find(hash_set* H, int key);
int contains(hash_set* H, int key);
hash_set* remove_entry(hash_set* H, int key);
void show(hash_set* H);
hash_set* rehash(hash_set* H);

int hash_function(hash_set* H, int key) {
    return key % (H->max_size);
}

int size(hash_set* H) {
    int size = 0;
    for (int i = 0; i < H->max_size; i++) {
        if (!((H->array) + i)->IsEmpty) {
            size++;
        }
    }
    return size;
}

float load_factor(hash_set* H) {
    return (float)size(H) / (H->max_size);
}

int IsFull(hash_set* H) {
    return load_factor(H) >= 1.0;
}

hash_set* add(hash_set* H, int entry) {
    if (IsFull(H)) {
        printf("Error: Hash set is full!\n");
        printf("Rehash the hash set? (1 for Yes, 0 for No): ");
        int choice;
        scanf("%d", &choice);
        if (choice == 0) {
            printf("Error: Hash set is full!\n");
            return H;
        } else if (choice == 1) {
            H = rehash(H);
            H = add(H, entry);
            show(H);
            return H;
        }
    } else {
        int hash_code = hash_function(H, entry);
        while (!((H->array) + hash_code)->IsEmpty) {
            hash_code++;
            if (hash_code == H->max_size) {
                hash_code = 0;
            }
        }
        ((H->array) + hash_code)->IsEmpty = 0;
        ((H->array) + hash_code)->value = entry;
        return H;
    }
    return H;
}

hash_set* CreateEmpty() {
    hash_set* H = (hash_set*)malloc(sizeof(hash_set));
    int n;
    printf("Give the hash set size: ");
    scanf("%d", &n);
    H->max_size = n;
    H->array = (bucket*)malloc(n * sizeof(bucket));
    for (int i = 0; i < H->max_size; i++) {
        ((H->array) + i)->IsEmpty = 1;
    }
    return H;
}

int find(hash_set* H, int key) {
    for (int i = 0; i < H->max_size; i++) {
        if (!((H->array) + i)->IsEmpty && ((H->array) + i)->value == key) {
            return i;
        }
    }
    return -1;
}

int contains(hash_set* H, int key) {
    return (find(H, key) != -1);
}

hash_set* remove_entry(hash_set* H, int key) {
    int pos = find(H, key);
    if (pos == -1) {
        printf("Error: non-existent key\n");
        exit(-1);
    } else {
        ((H->array) + pos)->IsEmpty = 1;
        return H;
    }
}

void show(hash_set* H) {
    int n = H->max_size;
    int empty = 1;
    for (int i = 0; i < n; i++) {
        if (!((H->array) + i)->IsEmpty) {
            printf("<%d>", ((H->array) + i)->value);
            empty = 0;
        } else {
            printf("<Empty>");
        }
    }
    if (empty) {
        printf("Empty hash set\n");
    }
    printf("\n");
}

hash_set* rehash(hash_set* H) {
    int old_size = H->max_size;
    bucket* old_array = H->array;

    H->max_size *= 2;
    H->array = (bucket*)malloc(H->max_size * sizeof(bucket));
    for (int i = 0; i < H->max_size; i++) {
        H->array[i].IsEmpty = 1;
    }

    for (int i = 0; i < old_size; i++) {
        if (!old_array[i].IsEmpty) {
            H=add(H, old_array[i].value);
        }
    }

    free(old_array);
    return H;
}

int main() {
    hash_set* H = CreateEmpty();
    add(H, 5);
    add(H, 10);
    show(H);
    add(H, 3);
    show(H);
    add(H, 7);
    show(H);
    return 0;
}