//
//  HHArray.c
//  HHArray
//
//  Created by Harlan Haskins on 4/21/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utilities.h"

#define ITEM_SIZE sizeof(void *)

typedef struct HHArray_S {
    size_t size;
    size_t capacity;
    void **values;
} * HHArray;

#define _HHARRAY_DEFINED_
#include "HHArray.h"
#undef _HHARRAY_DEFINED_

const size_t DEFAULT_CAPACITY = 10;
const size_t HHArrayNotFound = SIZE_MAX;
const double RESIZE_FACTOR = 1.5;
const double LOAD_THRESHOLD = 0.75;

#ifdef UNIT_TEST
#define EXIT_WITH_FAILURE exit(EXIT_FAILURE)
#else
#define EXIT_WITH_FAILURE
#endif

size_t min(size_t a, size_t b) {
    return a > b ? b : a;
}

size_t max(size_t a, size_t b) {
    return a > b ? a : b;
}

/**
 * Asserts that 'index' is less than 'highest', and otherwise causes an error and exits.
 */
static void assert_index(HHArray array, size_t highest, size_t index) {
    if (index > highest) {
        fprintf(stderr, "Array index %zu higher than highest index %zu.", index, highest);
        EXIT_WITH_FAILURE;
    }
}

#pragma mark - Creation and Destruction

HHArray hharray_create_capacity(size_t capacity) {
    if (capacity == 0) {
        fputs("Cannot initialize an hharray with capacity 0.\n", stderr);
        EXIT_WITH_FAILURE;
    }
    capacity = max(capacity, DEFAULT_CAPACITY);
    HHArray array = hhmalloc(sizeof(struct HHArray_S));
    array->capacity = capacity;
    array->size = 0;
    array->values = hhcalloc(array->capacity, ITEM_SIZE);
    return array;
}

HHArray hharray_create() {
    return hharray_create_capacity(DEFAULT_CAPACITY);
}

HHArray hharray_copy(HHArray array) {
    HHArray new = hharray_create_capacity(array->capacity);
    new->size = array->size;
    memcpy(new->values, array->values, array->size *ITEM_SIZE);
    return new;
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
        if (i < array->size - 1)
            fputs(", ", stdout);
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
    array->values = hhrealloc(array->values, new_capacity * ITEM_SIZE);
    array->capacity = new_capacity;
}

/**
 * Determines whether the HHArray is past the LOAD_THRESHOLD.
 */
static int hharray_should_grow(HHArray array) {
    return ((double)array->size / (double)array->capacity) > LOAD_THRESHOLD;
}

void hharray_ensure_capacity(HHArray array, size_t capacity) {
    if (array->capacity >= capacity) return;
    array->values = hhrealloc(array->values, capacity * ITEM_SIZE);
    array->capacity = capacity;
}

/**
 * Grows an HHArray by RESIZE_FACTOR.
 */
static void hharray_grow(HHArray array) {
    hharray_ensure_capacity(array, array->capacity * RESIZE_FACTOR);
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
    assert_index(array, array->size - 1, index);
    return array->values[index];
}

void hharray_insert_list(HHArray dest, HHArray source, size_t index) {
    assert_index(dest, dest->size - 1, index);
    hharray_ensure_capacity(dest, dest->capacity + source->capacity);
    void *old_value_dst = &dest->values[index + source->size];
    void *input_index = &dest->values[index];
    void *new_values = source->values;
    memmove(old_value_dst, input_index, (source->size * ITEM_SIZE));
    memcpy(input_index, new_values, (source->size * ITEM_SIZE));
    dest->size += source->size;
}

void hharray_append_list(HHArray dest, HHArray source) {
    hharray_ensure_capacity(dest, dest->capacity + source->capacity);
    void *dst = &dest->values[dest->size];
    void *src = source->values;
    memcpy(dst, src, (source->size * ITEM_SIZE));
    dest->size += source->size;
}

void hharray_insert_index(HHArray array, void *value, size_t index) {
    assert_index(array, array->size, index);
    if (hharray_should_grow(array)) {
        hharray_grow(array);
    }
    void *dst = &array->values[index + 1];
    void *src = &array->values[index];
    memmove(dst, src, ((array->size - index) * ITEM_SIZE));
    array->values[index] = value;
    array->size++;
}

void *hharray_remove_index(HHArray array, size_t index) {
    void *value = hharray_get(array, index);
    array->values[index] = NULL;
    int is_last = (index == array->size - 1);
    array->size--;
    if (is_last) return value;
    void *dst = &array->values[index];
    void *src = &array->values[index + 1];
    memmove(dst, src, ((array->size - index) * ITEM_SIZE));
    if (hharray_should_shrink(array)) {
        hharray_shrink(array);
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
    assert_index(array, array->size - 1, first_index);
    assert_index(array, array->size - 1, second_index);
    void *first = array->values[first_index];
    void *second = array->values[second_index];
    array->values[first_index] = second;
    array->values[second_index] = first;
}

void hharray_shuffle(HHArray array) {
    if (array->size <= 1) return;
    if (array->size == 2) {
        hharray_swap(array, 0, 1);
        return;
    }
    for (size_t i = array->size - 1; i > 0; --i) {
        size_t swap = rand() / (RAND_MAX / i + 1);
        hharray_swap(array, swap, (int)i);
    }
}

void hharray_reverse(HHArray array) {
    if (array->size <= 1) return;
    for (size_t i = 0; i < array->size / 2; i++) {
        hharray_swap(array, i, array->size - i - 1);
    }
}

void hharray_sort(HHArray array, int (*comparison)(const void *a, const void *b)) {
    if (array->size <= 1) return;
    qsort(array->values, array->size, ITEM_SIZE, comparison);
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

int equals(void *a, void *b) {
    return a == b;
}

void *hharray_remove_f(HHArray array, void *element, int (*comparison)(void *, void *)) {
    size_t index = hharray_find_f(array, element, comparison);
    if (index == HHArrayNotFound) {
        fprintf(stderr, "Element <%p> not found in array <%p>", element, array);
        EXIT_WITH_FAILURE;
    }
    return hharray_remove_index(array, index);
}

void *hharray_remove(HHArray array, void *element) {
    return hharray_remove_f(array, element, NULL);
}

HHArray hharray_slice(HHArray array, size_t first, size_t second) {
    size_t start = min(first, second);
    size_t end = max(first, second);
    assert_index(array, array->size - 1, start);
    assert_index(array, array->size - 1, end);
    size_t num_elements = (end - start);
    size_t new_capacity = max(num_elements / LOAD_THRESHOLD, 1);
    HHArray new = hharray_create_capacity(new_capacity);
    void *src = &array->values[start];
    void *dst = new->values;
    memcpy(dst, src, num_elements * sizeof(void *));
    new->size = num_elements;
    if (first > second) {
        hharray_reverse(new);
    }
    return new;
}

size_t hharray_find_f(HHArray array, void *element, int (*comparison)(void *, void *)) {
    if (array->size == 0) return HHArrayNotFound;
    for (size_t i = 0; i < array->size; i++) {
        if ((comparison ? comparison : equals)(element, array->values[i])) {
            return i;
        }
    }
    return HHArrayNotFound;
}

size_t hharray_find(HHArray array, void *element) {
    return hharray_find_f(array, element, equals);
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
    if (hharray_should_shrink(new)) {
        hharray_shrink(new);
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
    void **new = hhcalloc(array->size, ITEM_SIZE);
    for (size_t i = 0; i < array->size; i++) {
        new[i] = array->values[i];
    }
    return new;
}
