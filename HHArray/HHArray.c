//
//  HHArray.c
//  HHArray
//
//  Created by Harlan Haskins on 4/21/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#include <stdlib.h>
#include "utilities.h"

typedef struct HHArray_S {
    size_t size;
    size_t capacity;
    void **values;
} *HHArray;

#define _HHARRAY_DEFINED_
#include "HHArray.h"
#undef _HHARRAY_DEFINED_

size_t DEFAULT_CAPACITY = 10;
double RESIZE_FACTOR = 1.5;
double LOAD_THRESHOLD = 0.75;

HHArray hharray_create_capacity(size_t capacity) {
    if (capacity == 0) {
        fputs("Cannot initialize an hharray with capacity 0.", stderr);
        exit(EXIT_FAILURE);
    }
    HHArray array = hhmalloc(sizeof(struct HHArray_S));
    array->capacity = capacity;
    array->size = 0;
    array->values = hhcalloc(array->capacity, sizeof(void *));
    return array;
}

void assert_index(size_t highest, size_t index) {
    if (index > highest) {
        fprintf(stderr, "Array index %zu higher than highest index %zu.", index, highest);
        exit(EXIT_FAILURE);
    }
}

HHArray hharray_create() {
    return hharray_create_capacity(DEFAULT_CAPACITY);
}

void hharray_destroy(HHArray array) {
    free(array->values);
    free(array);
}

static int hharray_should_grow(HHArray array) {
    return ((double)array->size / (double)array->capacity) > LOAD_THRESHOLD;
}

static void hharray_grow(HHArray array) {
    size_t new_capacity = array->capacity * RESIZE_FACTOR;
    array->values = hhrealloc(array->values, new_capacity * sizeof(void *));
    printf("Growing from %zu to %zu\n", array->capacity, new_capacity);
    array->capacity = new_capacity;
}

void _print_ptr(void *ptr) {
    printf("<%8p>", ptr);
}

void hharray_print_f(HHArray array, void (*print)(void *)) {
    putchar('[');
    for (size_t i = 0; i < array->size; i++) {
        (print ?: _print_ptr)(array->values[i]);
        if (i < array->size - 1) fputs(", ", stdout);
    }
    puts("]");
}

void hharray_print(HHArray array) {
    hharray_print_f(array, NULL);
}

static int hharray_should_shrink(HHArray array) {
    size_t capacity_after_shrink = array->capacity / RESIZE_FACTOR;
    double load_after_shrink = (double)array->size / (double)capacity_after_shrink;
    int should_shrink = load_after_shrink < LOAD_THRESHOLD && capacity_after_shrink > 0;
    return should_shrink;
}

static void hharray_shrink(HHArray array) {
    size_t new_capacity = array->capacity / RESIZE_FACTOR;
    array->values = hhrealloc(array->values, new_capacity * sizeof(void *));
    printf("Shrinking from %zu to %zu\n", array->capacity, new_capacity);
    array->capacity = new_capacity;
}

size_t hharray_size(HHArray array) {
    return array->size;
}

void hharray_swap(HHArray array, size_t first_index, size_t second_index) {
    void *first = array->values[first_index];
    void *second = array->values[second_index];
    array->values[first_index] = second;
    array->values[second_index] = first;
}

int _random_int_in_range(size_t min, size_t max) {
    double scaled = (double)rand()/ RAND_MAX;
    int random = (max - min + 1) * scaled + min;
    return random;
}

void hharray_shuffle(HHArray array) {
    int swap_index = 0;
    
    for (size_t i = array->size - 1; i > 0; --i) {
        if (array->size == 2) {
            hharray_swap(array, 0, 1);
            return;
        }
        
        swap_index = _random_int_in_range(0, i);
        hharray_swap(array, swap_index, (int)i);
    }
}

void hharray_sort(HHArray array, int (*comparison)(const void *a, const void *b)) {
    qsort(array->values, array->size, sizeof(void *), comparison);
}

void hharray_append(HHArray array, void *value) {
    if (hharray_should_grow(array)) {
        hharray_grow(array);
    }
    array->values[array->size] = value;
    array->size++;
}

void *hharray_get(HHArray array, size_t index) {
    assert_index(array->size - 1, index);
    return array->values[index];
}

void hharray_insert_index(HHArray array, void *value, size_t index) {
    assert_index(array->size, index);
    if (hharray_should_grow(array)) {
        hharray_grow(array);
    }
    for (size_t shift = array->size; shift > index; shift--) {
        array->values[shift] = array->values[shift - 1];
    }
    array->values[index] = value;
    array->size++;
}

void *hharray_remove_index(HHArray array, size_t index) {
    if (hharray_should_shrink(array)) {
        hharray_shrink(array);
    }
    void *value = hharray_get(array, index);
    array->values[index] = NULL;
    for (size_t i = index + 1; i < array->size; i++) {
        array->values[i - 1] = array->values[i];
    }
    array->size--;
    return value;
}

void *hharray_pop(HHArray array) {
    return hharray_remove_index(array, 0);
}

void hharray_push(HHArray array, void *value) {
    hharray_insert_index(array, value, 0);
}

void hharray_enqueue(HHArray array, void *value) {
    hharray_append(array, value);
}

void *hharray_dequeue(HHArray array) {
    return hharray_pop(array);
}