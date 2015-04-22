//
//  utilities.c
//
//  Created by Harlan Haskins on 4/4/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#include <stdlib.h>
#include "utilities.h"

void *hhcalloc(size_t count, size_t size) {
    void *memory = calloc(count, size);
    if (memory == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    return memory;
}

void *hhmalloc(size_t size) {
    return hhcalloc(1, size);
}

void *hhrealloc(void *src, size_t size) {
    void *new = realloc(src, size);
    if (new == NULL) {
        free(src);
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    return new;
}

void freelist_f(void **list, size_t size, void (*free_func)(void *)) {
    for (size_t i = 0; i < size; i++) {
        (free_func ? free_func : free)(list[i]);
    }
    free(list);
}

void freelist(void **list, size_t size) {
    freelist_f(list, size, NULL);
}

