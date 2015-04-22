//
//  main.c
//  HHArray
//
//  Created by Harlan Haskins on 4/21/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "HHArray.h"

#define CASTREF(Type, x) (*(Type *)x)
#define printsep(s) fputs("\n\n===== "s" =====\n\n", stdout)

void print(void *ptr) {
    printf("%ld", (long)ptr);
}

int cmpfunc(const void *a, const void *b) {
    return (int)(CASTREF(long, a) - CASTREF(long, b));
}

void *double_ptr(void *a) {
    return (void *)((long)a * 2);
}

void *add_long(void *a, void *b) {
    return (void *)((long)a + (long)b);
}

int is_even(void *a) {
    return (long)a % 2 == 0;
}

void test_sort(HHArray array) {
    printsep("Testing Sorting");
    hharray_print_f(array, print);
    printf("\n\nSorted? %s\n\n", hharray_is_sorted(array, cmpfunc) ? "yes" : "no");
    hharray_sort(array, cmpfunc);
    hharray_print_f(array, print);
    printf("\n\nSorted? %s", hharray_is_sorted(array, cmpfunc) ? "yes" : "no");
}

void test_shuffle(HHArray array) {
    printsep("Testing Shuffle");
    hharray_shuffle(array);
    hharray_print_f(array, print);
}

void test_map(HHArray array) {
    printsep("Testing Map");
    HHArray doubled = hharray_map(array, double_ptr);
    fputs("Doubled: ", stdout);
    hharray_print_f(doubled, print);
    hharray_destroy(doubled);
}

void test_filter(HHArray array) {
    printsep("Testing Filter");
    HHArray evens = hharray_filter(array, is_even);
    fputs("Even numbers: ", stdout);
    hharray_print_f(evens, print);
    hharray_destroy(evens);
}

void test_reduce(HHArray array) {
    printsep("Testing Reduce");
    long sum = (long)hharray_reduce(array, NULL, add_long);
    printf("Sum: %ld", sum);
}

void test_pointer_print(HHArray array) {
    printsep("Testing Pointer Print");
    hharray_print(array);
}

void test_append(HHArray array) {
    printsep("Testing Append");
    for (size_t i = 0; i < 100; i++) {
        hharray_append(array, (void *)(long)(rand() % 100));
    }
    hharray_print_f(array, print);
    assert(hharray_size(array) == 100);
}

int main() {
    srand((unsigned int)time(0));
    HHArray array = hharray_create();
    
    test_append(array);
    test_pointer_print(array);
    test_sort(array);
    test_shuffle(array);
    test_map(array);
    test_filter(array);
    test_reduce(array);

    hharray_destroy(array);
    
    putchar('\n');

    return 0;
}

