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
#include "HHArray.h"

#define CASTREF(Type, x) (*(Type *)x)

void print(void *ptr) {
    printf("%ld", (long)ptr);
}

int cmpfunc(const void *a, const void *b) {
    return (int)(CASTREF(long, a) - CASTREF(long, b));
}

void *double_ptr(void *a) {
    return (void *)((long)a * 2);
}

int is_even(void *a) {
    return (long)a % 2 == 0;
}

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(0));
    HHArray array = hharray_create();
    for (size_t i = 0; i < 100; i++) {
        hharray_append(array, (void *)(long)(rand() % 100));
    }
    hharray_print_f(array, print);
    hharray_print(array);
    hharray_sort(array, cmpfunc);
    hharray_print_f(array, print);
    HHArray doubled = hharray_map(array, double_ptr);
    fputs("Doubled: ", stdout);
    hharray_print_f(doubled, print);
    printf("Sorted? %s\n", hharray_is_sorted(array, cmpfunc) ? "yes" : "no");
    hharray_destroy(doubled);
    
    HHArray evens = hharray_filter(array, is_even);
    fputs("Even numbers: ", stdout);
    hharray_print_f(evens, print);
    hharray_destroy(evens);
    hharray_shuffle(array);
    hharray_print_f(array, print);
    hharray_shuffle(array);
    hharray_print_f(array, print);
    hharray_destroy(array);
    
    return 0;
}
