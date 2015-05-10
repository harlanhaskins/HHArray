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
#include <pthread.h>

#define UNIT_TEST (Needed so tests keep running)
#include "HHArray.h"
#undef UNIT_TEST

#define CASTREF(Type, x) (*(Type *)x)
#define printtest(s) fputs("\n\n===== Testing " s " =====\n\n", stdout)

#define NSEC_PER_SEC 1e9
#define NSEC_PER_MILLISEC 1e6
#define NSEC_PER_MICROSEC 1e3

void time_test(void (*test_func)()) {
    clock_t start = clock();
    test_func();
    clock_t end = clock();

    double elapsed = end - start;
    char *unit = "ns";
    if (elapsed > NSEC_PER_SEC) {
        elapsed /= NSEC_PER_SEC;
        unit = "s";
    } else if (elapsed > NSEC_PER_MILLISEC) {
        elapsed /= NSEC_PER_MILLISEC;
        unit = "ms";
    } else if (elapsed > NSEC_PER_MICROSEC) {
        elapsed /= NSEC_PER_MICROSEC;
        unit = "μs";
    }
    printf("\nelapsed time: %.3f%s\n", elapsed, unit);
}

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

void fill_array(HHArray array, size_t count) {
    for (size_t i = 0; i < count; i++) {
        hharray_append(array, (void *)(long)(rand() % 100));
    }
}

void test_sort() {
    printtest("Sorting");
    HHArray array = hharray_create();
    fill_array(array, 100);
    hharray_print_f(array, print);
    printf("\n\nSorted? %s\n\n",
           hharray_is_sorted(array, cmpfunc) ? "yes" : "no");
    hharray_sort(array, cmpfunc);
    hharray_print_f(array, print);
    printf("\n\nSorted? %s", hharray_is_sorted(array, cmpfunc) ? "yes" : "no");
    hharray_destroy(array);
}

void test_shuffle() {
    printtest("Shuffle");
    HHArray array = hharray_create();
    fill_array(array, 100);
    fputs("Original: ", stdout);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_shuffle(array);
    fputs("Shuffled: ", stdout);
    hharray_print_f(array, print);
    hharray_destroy(array);
}

void test_map() {
    printtest("Map");
    HHArray array = hharray_create();
    fill_array(array, 100);
    fputs("Original: ", stdout);
    hharray_print_f(array, print);
    putchar('\n');
    HHArray doubled = hharray_map(array, double_ptr);
    fputs("Doubled: ", stdout);
    hharray_print_f(doubled, print);
    hharray_destroy(doubled);
    hharray_destroy(array);
}

void test_filter() {
    printtest("Filter");
    HHArray array = hharray_create();
    fill_array(array, 100);
    fputs("Full: ", stdout);
    hharray_print_f(array, print);
    putchar('\n');
    HHArray evens = hharray_filter(array, is_even);
    fputs("Evens: ", stdout);
    hharray_print_f(evens, print);
    hharray_destroy(evens);
    hharray_destroy(array);
}

void test_reduce() {
    printtest("Reduce");
    HHArray array = hharray_create();
    fill_array(array, 100);
    long sum = (long)hharray_reduce(array, 0, add_long);
    printf("Sum: %ld", sum);
    hharray_destroy(array);
}

void test_pointer_print() {
    printtest("Pointer Print");
    HHArray array = hharray_create();
    fill_array(array, 10);
    hharray_print(array);
    hharray_destroy(array);
}

void test_append() {
    printtest("Append");
    HHArray array = hharray_create();
    fill_array(array, 100);
    hharray_print_f(array, print);
    assert(hharray_size(array) == 100);
    hharray_destroy(array);
}

void test_insert() {
    printtest("Insert");
    HHArray array = hharray_create();
    fill_array(array, 10);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_insert_index(array, (void *)(long)(rand() % 100), 1);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_insert_index(array, (void *)(long)(rand() % 100), 1);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_destroy(array);
}

void test_remove() {
    printtest("Remove");
    HHArray array = hharray_create();
    fill_array(array, 10);
    void *value_to_find = (void *)4;
    hharray_insert_index(array, value_to_find, 4);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_remove(array, value_to_find);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_destroy(array);
}

void test_remove_index() {
    printtest("Remove Index");
    HHArray array = hharray_create();
    fill_array(array, 10);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_remove_index(array, 1);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_remove_index(array, 5);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_destroy(array);
}

void test_copy() {
    printtest("Copy");
    HHArray array = hharray_create();
    fill_array(array, 10);
    HHArray copy = hharray_copy(array);
    printf("Original: ");
    hharray_print_f(array, print);
    printf("\nCopy: ");
    hharray_print_f(copy, print);
    printf("\nAppending 5 to original...\n");
    hharray_append(array, (void *)5);
    printf("Original: ");
    hharray_print_f(array, print);
    printf("\nCopy: ");
    hharray_print_f(copy, print);
    assert(hharray_size(array) != hharray_size(copy));
    hharray_destroy(array);
    hharray_destroy(copy);
}

void test_append_list() {
    printtest("Append List");
    HHArray src = hharray_create();
    HHArray dst = hharray_create();
    for (size_t i = 0; i < 10; i++) {
        hharray_append(src, (void *)(long)(rand() % 100));
        hharray_append(dst, (void *)(long)(rand() % 100));
    }
    printf("Source: ");
    hharray_print_f(src, print);
    printf("\nDestination: ");
    hharray_print_f(dst, print);
    putchar('\n');
    hharray_append_list(dst, src);
    printf("\nCombined: ");
    hharray_print_f(dst, print);
    putchar('\n');
    hharray_destroy(src);
    hharray_destroy(dst);
}

void test_insert_list() {
    printtest("Insert List");
    HHArray src = hharray_create();
    HHArray dst = hharray_create();
    for (size_t i = 0; i < 10; i++) {
        hharray_append(src, (void *)(long)(rand() % 100));
        hharray_append(dst, (void *)(long)(rand() % 100));
    }
    printf("Source: ");
    hharray_print_f(src, print);
    printf("\nDestination: ");
    hharray_print_f(dst, print);
    putchar('\n');
    hharray_insert_list(dst, src, 5);
    printf("\nCombined: ");
    hharray_print_f(dst, print);
    putchar('\n');
    hharray_destroy(src);
    hharray_destroy(dst);
}

void test_reverse() {
    printtest("Reverse");
    HHArray array = hharray_create();
    fill_array(array, 11);
    fputs("Original: ", stdout);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_reverse(array);
    fputs("Reversed: ", stdout);
    hharray_print_f(array, print);
    putchar('\n');
    hharray_destroy(array);
}

void test_slice() {
    printtest("Slice");
    HHArray array = hharray_create();
    fill_array(array, 11);
    fputs("Original: ", stdout);
    hharray_print_f(array, print);
    putchar('\n');
    HHArray sliced = hharray_slice(array, 3, 9);
    fputs("Sliced: ", stdout);
    hharray_print_f(sliced, print);
    putchar('\n');
    hharray_destroy(array);
    hharray_destroy(sliced);
}

void test_stress() {
    printtest("Stress");
    HHArray array = hharray_create();
    fill_array(array, 100000);
    while (hharray_size(array) > 70000) {
        size_t index = (size_t)(rand() % hharray_size(array));
        hharray_remove_index(array, index);
    }
    while (hharray_size(array) > 100) {
        hharray_pop(array);
    }
    hharray_print_f(array, print);
    hharray_destroy(array);
}

void print_char(void *c) {
    printf("%c", (char)c);
}

void test_string() {
    printtest("String");
    HHArray string = hharray_create();
    hharray_append(string, (void *)'c');
    hharray_append(string, (void *)'h');
    hharray_append(string, (void *)'a');
    hharray_append(string, (void *)'r');
    hharray_append(string, (void *)'t');
    hharray_append(string, (void *)'s');
    hharray_append(string, (void *)'\0');
    hharray_print_f(string, print_char);
    hharray_destroy(string);
}

int main() {
    srand((unsigned int)time(0));
    time_test(test_append);
    time_test(test_pointer_print);
    time_test(test_sort);
    time_test(test_shuffle);
    time_test(test_map);
    time_test(test_filter);
    time_test(test_reduce);
    time_test(test_insert);
    time_test(test_insert_list);
    time_test(test_remove);
    time_test(test_remove_index);
    time_test(test_copy);
    time_test(test_reverse);
    time_test(test_slice);
    time_test(test_append_list);
    time_test(test_string);
    time_test(test_stress);
    putchar('\n');

    return 0;
}
