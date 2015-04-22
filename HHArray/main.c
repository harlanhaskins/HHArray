//
//  main.c
//  HHArray
//
//  Created by Harlan Haskins on 4/21/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "HHArray.h"

#define CASTREF(Type, x) (*(Type *)x)

void print(void *ptr) {
    printf("%ld", (long)ptr);
}

int cmpfunc (const void *a, const void *b) {
    return (long)a > (long)b;
}

int main(int argc, const char * argv[]) {
    HHArray array = hharray_create();
    for (size_t i = 0; i < 10; i++) {
        hharray_push(array, (void *)(long)(rand() % 100));
    }
    hharray_print_f(array, print);
    hharray_sort(array, cmpfunc);
    hharray_print_f(array, print);
    hharray_shuffle(array);
    hharray_print_f(array, print);
    hharray_shuffle(array);
    hharray_print_f(array, print);
    hharray_destroy(array);
    
    return 0;
}
