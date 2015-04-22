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

/**
 * Asserts that 'index' is less than 'highest', and otherwise causes an error and exits.
 */
static void assert_index(size_t highest, size_t index) {
    if (index > highest) {
        fprintf(stderr, "Array index %zu higher than highest index %zu.", index, highest);
        exit(EXIT_FAILURE);
    }
}

#pragma mark - Creation and Destruction

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


HHArray hharray_create() {
    return hharray_create_capacity(DEFAULT_CAPACITY);
}

void hharray_destroy(HHArray array) {
    free(array->values);
    free(array);
}

#pragma mark - Printing

/**
 * Prints a (void *) with %p.
 */
static void _print_ptr(void *ptr) {
    printf("<%p>", ptr);
}

void hharray_print_f(HHArray array, void (*print)(void *)) {
    putchar('[');
    for (size_t i = 0; i < array->size; i++) {
        (print ? print : _print_ptr)(array->values[i]);
        if (i < array->size - 1) fputs(", ", stdout);
    }
    putchar(']');
}

void hharray_print(HHArray array) {
    hharray_print_f(array, NULL);
}

#pragma mark - Internal Resizing

/**
 * Determines whether shrinking the HHArray will keep it within the LOAD_THRESHOLD.
 */
static int hharray_should_shrink(HHArray array) {
    size_t capacity_after_shrink = array->capacity / RESIZE_FACTOR;
    double load_after_shrink = (double)array->size / (double)capacity_after_shrink;
    int should_shrink = load_after_shrink < LOAD_THRESHOLD && capacity_after_shrink > 0;
    return should_shrink;
}

/**
 * Shrinks an HHArray by RESIZE_FACTOR.
 */
static void hharray_shrink(HHArray array) {
    size_t new_capacity = array->capacity / RESIZE_FACTOR;
    array->values = hhrealloc(array->values, new_capacity * sizeof(void *));
    array->capacity = new_capacity;
}

/**
 * Determines whether the HHArray is past the LOAD_THRESHOLD.
 */
static int hharray_should_grow(HHArray array) {
    return ((double)array->size / (double)array->capacity) > LOAD_THRESHOLD;
}

/**
 * Grows an HHArray by RESIZE_FACTOR.
 */
static void hharray_grow(HHArray array) {
    size_t new_capacity = array->capacity * RESIZE_FACTOR;
    array->values = hhrealloc(array->values, new_capacity * sizeof(void *));
    array->capacity = new_capacity;
}

size_t hharray_size(HHArray array) {
    return array->size;
}

#pragma mark - Insertion and Removal

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
    array->size--;
    if (index == array->size - 1) return value;
    for (size_t i = index + 1; i < array->size; i++) {
        array->values[i - 1] = array->values[i];
    }
    return value;
}

#pragma mark - Stack Functions

void hharray_push(HHArray array, void *value) {
    hharray_insert_index(array, value, 0);
}

void *hharray_pop(HHArray array) {
    return hharray_remove_index(array, 0);
}

#pragma mark - Queue Functions

void hharray_enqueue(HHArray array, void *value) {
    hharray_append(array, value);
}

void *hharray_dequeue(HHArray array) {
    return hharray_pop(array);
}

#pragma mark - Utilities

void hharray_swap(HHArray array, size_t first_index, size_t second_index) {
    void *first = array->values[first_index];
    void *second = array->values[second_index];
    array->values[first_index] = second;
    array->values[second_index] = first;
}

void hharray_shuffle(HHArray array) {
    if (array->size == 2) {
        hharray_swap(array, 0, 1);
        return;
    }
    for (size_t i = array->size - 1; i > 0; --i) {
        size_t swap = rand() / (RAND_MAX / i + 1);
        hharray_swap(array, swap, (int)i);
    }
}

void hharray_sort(HHArray array, int (*comparison)(const void *a, const void *b)) {
    qsort(array->values, array->size, sizeof(void *), comparison);
}

int hharray_is_sorted(HHArray array, int (*comparison)(const void *a, const void *b)) {
    if (array->size <= 1) return 1;
    for (size_t i = 0; i < (array->size - 1); i++) {
        if (comparison(&array->values[i], &array->values[i + 1]) > 0) {
            return 0;
        }
    }
    return 1;
}

#pragma mark - Functional Abstractions

HHArray hharray_map(HHArray array, void *(*transform)(void *)) {
    HHArray new = hharray_create_capacity(array->size);
    for (size_t i = 0; i < array->size; i++) {
        void *new_value = transform(array->values[i]);
        hharray_append(new, new_value);
    }
    return new;
}

HHArray hharray_filter(HHArray array, int (*include)(void *)) {
    HHArray new = hharray_create_capacity(array->size);
    for (size_t i = 0; i < array->size; i++) {
        if (include(array->values[i])) {
            hharray_append(new, array->values[i]);
        }
    }
    return new;
}

void *hharray_reduce(HHArray array, void *initial, void *(*combine)(void *, void *)) {
    void *current = initial;
    for (size_t i = 0; i < array->size; i++) {
        current = combine(current, array->values[i]);
    }
    return current;
}

void **hharray_values(HHArray array) {
    void **new = hhcalloc(array->size, sizeof(void *));
    for (size_t i = 0; i < array->size; i++) {
        new[i] = array->values[i];
    }
    return new;
}

