//
//  HHArray.h
//  HHArray
//
//  Created by Harlan Haskins on 4/21/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#ifndef __HHArray__HHArray__
#define __HHArray__HHArray__

#include <stdio.h>

#ifndef _HHARRAY_DEFINED_
typedef struct { } *HHArray;
#endif

/**
 * Initializes an HHArray with a given capacity.
 * If you plan on using the array to store many values,
 * it helps to initialize it with a large capacity to 
 * avoid unnecessarily growing the list.
 */
HHArray hharray_create_capacity(size_t capacity);

/**
 * Initializes an empty HHArray with a default capacity.
 */
HHArray hharray_create();

/**
 * Frees an HHArray.
 * @note This does not free any of the values contained
 * in the array. If you need to free the values contained,
 * you can use `hharray_values()` to get a `(void **)` with
 * all the pointers stored in this array.
 */
void hharray_destroy(HHArray array);

/**
 * @return the number of items currently stored in the array.
 */
size_t hharray_size(HHArray array);

/**
 * Appends the `value` at the end of the array's storage.
 */
void hharray_append(HHArray array, void *value);

/**
 * @return the value held at `index` in the array.
 * @note if the array is smaller than the requested index,
 *       HHArray will print an error message and exit.
 */
void *hharray_get(HHArray array, size_t index);

/**
 * Inserts the provided value at a given index in the array.
 * @note requires an O(n) swap from `index` to the array's size,
         in order to shift the values.
 * @note if the provided `index` is greater than the array's size,
         then HHArray will print an error message and exit.
 */
void hharray_insert_index(HHArray array, void *value, size_t index);

/**
 * Removes a value at an index and returns the removed value.
 * @note if index is greater than the last index in the array,
 *       HHArray will print an error message and exit.
 * @note requires a reverse O(n) swap from the array's size to `index`,
 *       in order to shift the values.
 * @return the removed value.
 */
void *hharray_remove_index(HHArray array, size_t index);

/**
 * Removes the first value from the list.
 * @note requires a complete reverse O(n) swap from the array's size,
 *       in order to shift the values.
 * @return the removed value.
 */
void *hharray_pop(HHArray array);

/**
 * Inserts the value at the beginning of the list.
 * @note requires a complete O(n) swap of each existing
 *       element in the array in order to shift the values.
 */
void hharray_push(HHArray array, void *value);

/**
 * Prints the contents of the array using the specified print
 * function on each value.
 */
void hharray_print_f(HHArray array, void (*print)(void *));

/**
 * Prints the contents of the array by printing each value as a pointer.
 */
void hharray_print(HHArray array);

/**
 * Sorts an array using the provided comparison function on each value.
 * @param array The array
 * @param comparison A comparison function used for sorting.
 *                   The comparison function must return an integer
 *                   less than, equal to, or greater than zero if the
 *                   first argument is considered to be respectively
 *                   less than, equal to, or greater than the second.
 */
void hharray_sort(HHArray array, int (*comparison)(const void *a, const void *b));

/**
 * Shuffles the array using a Fischer-Yates shuffle.
 */
void hharray_shuffle(HHArray array);

/**
 * @return `true` if the array is sorted as per the comparison function.
 * @note Not functional, so hidden.
 */
// int hharray_is_sorted(HHArray array, int (*comparison)(const void *a, const void *b));

/**
 * Swaps the values at the provided indices.
 */
void hharray_swap(HHArray array, size_t first_index, size_t second_index);

#endif /* defined(__HHArray__HHArray__) */
