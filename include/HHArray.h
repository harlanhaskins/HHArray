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

extern const size_t HHArrayNotFound;

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
 * Copies the contents of the provided array into a new HHArray.
 * Modifying the copy will not modify the original, however modifying the values
 * in the copy will modify the original array.
 * @param array the array to copy
 * @return a shallow copy of 'array'
 * @note `O(n)`
 */
HHArray hharray_copy(HHArray array);

/**
 * Inserts the full contents of an array into an existing array at a given index.
 * @param dest the destination which will hold the combined values.
 * @param source the array to insert into `dest`.
 * @param index the index at which to insert the values.
 * @note `O(n)`
 */
void hharray_insert_list(HHArray dest, HHArray source, size_t index);

/**
 * Searches the array for the provided value by comparing pointers directly.
 * @param array the array to search
 * @param the element to search for
 * @return The index of `element` in `array`, or `HHArrayNotFound` if
 *         `element` doesn't exist in `array`.
 * @note `O(n)`
 */
size_t hharray_find(HHArray array, void *element);

/**
 * Searches the array for the provided value by comparing values using a comparison function.
 * @param array the array to search
 * @param the element to search for
 * @param is_equal a function used to check equality of two void *'s.
 * @return The index of `element` in `array`, or `HHArrayNotFound` if
 *         `element` doesn't exist in `array`.
 * @note `O(n)`
 */
size_t hharray_find_f(HHArray array, void *element, int (*is_equal)(void *, void *));

/**
 * Removes the provided element from the array by comparing pointers directly.
 * @param array the array to remove from.
 * @param element the element to remove.
 * @note If element does not exist in `array`, the function
 *       prints an error message and exits with a failure code.
 * @note `O(n)`
 */
void *hharray_remove(HHArray array, void *element);

/**
 * Removes the provided element from the array by comparing values using a comparison function.
 * @param array the array to remove from.
 * @param element the element to remove.
 * @param is_equal a function used to check equality of two void *'s.
 * @note If element does not exist in `array`, the function
 *       prints an error message and exits with a failure code.
 * @note `O(n)`
 */
void *hharray_remove_f(HHArray array, void *element, int (*is_equal)(void *, void *));

/**
 * Frees an HHArray.
 * @note This does not free any of the values contained
 * in the array. If you need to free the values contained,
 * you can use `hharray_values()` to get a `(void **)` with
 * all the pointers stored in this array.
 * @note `O(1)`
 */
void hharray_destroy(HHArray array);

/** 
 * A generic map function for entries of the array.
 * Creates a new array containing the result of applying
 * `transform` to all elements in the provided array.
 * @param array The array whose elements are to be transformed.
 * @param transform A transform of type (void *) -> (void *)
 *                  This transform will be applied to all
 *                  elements of the array in order.
 * @return A new HHArray containing the result of applying `transform`
 *         to each value in the provided array.
 * @note `O(n)`
 */
HHArray hharray_map(HHArray array, void *(*transform)(void *));

/**
 * A generic filter function for entries of the array.
 * Creates a new array containing the only the elements of `array`
 * that returned non-zero from the provided `include` function.
 * @param array The array whose elements are to be filtered.
 * @param include A function that decides whether or not to
 *                include a (void *) in the new array.
 * @return A new HHArray containing only the values from the first
 *         array that returned
 * @note `O(n)`
 */
HHArray hharray_filter(HHArray array, int (*include)(void *));

/**
 * A generic reduce function for entries of the array.
 * Continually applies `combine` to sequential values in the
 * array, 'reducing' it to one value.
 * @param array The array whose elements are to be transformed.
 * @param initial An initial value, to start the combination.
 * @param combine A function to combine two `(void *)`s into one.
 * @return A new (void *) containing the final combined value.
 * @note `O(n)`
 */
void *hharray_reduce(HHArray array, void *initial, void *(*combine)(void *, void *));

/**
 * Returns all the values contained in the array.
 * @param array The array whose elements are to be transformed.
 * @return A (void **) containing a shallow copy of the values in the array.
 * @note You must free the returned values list from this function.
 * @note You must use `hharray_size()` to find the length of this list.
 * @note `O(n)`
 */
void **hharray_values(HHArray array);

/**
 * @return the number of items currently stored in the array.
 * @note `O(1)`
 */
size_t hharray_size(HHArray array);

/**
 * Appends the `value` at the end of the array's storage.
 * @note `O(1)`
 */
void hharray_append(HHArray array, void *value);

/**
 * Appends each value in `source` at the end of `dest`'s storage.
 * @note `O(n)` where n is the source lists's size.
 */
void hharray_append_list(HHArray dest, HHArray source);

/**
 * @return the value held at `index` in the array.
 * @note if the array is smaller than the requested index,
 *       HHArray will print an error message and exit.
 * @note `O(1)`
 */
void *hharray_get(HHArray array, size_t index);

/**
 * Inserts the provided value at a given index in the array.
 * @note if the provided `index` is greater than the array's size,
 *       then HHArray will print an error message and exit.
 * @note requires an `O(n)` swap from `index` to the array's size,
 *       in order to shift the values.
 */
void hharray_insert_index(HHArray array, void *value, size_t index);

/**
 * Removes a value at an index and returns the removed value.
 * @return the removed value.
 * @note if index is greater than the last index in the array,
 *       HHArray will print an error message and exit.
 * @note requires a reverse `O(n)` swap from the array's size to `index`,
 *       in order to shift the values.
 */
void *hharray_remove_index(HHArray array, size_t index);

/**
 * Removes the first value from the list.
 * @return the removed value.
 * @note requires a complete reverse `O(n)` swap from the array's size,
 *       in order to shift the values.
 */
void *hharray_pop(HHArray array);

/**
 * Inserts the value at the beginning of the list.
 * @note requires a complete `O(n)` swap of each existing
 *       element in the array in order to shift the values.
 */
void hharray_push(HHArray array, void *value);

/**
 * Appends the `value` at the end of the array's storage.
 * @note `O(1)`
 */
void hharray_enqueue(HHArray array, void *value);

/**
 * Removes the first value from the list.
 * @return the removed value.
 * @note requires a complete reverse `O(n)` swap from the array's size,
 *       in order to shift the values.
 */
void *hharray_dequeue(HHArray array);

/**
 * Prints the contents of the array using the specified print
 * function on each value.
 * @note `O(n)`
 */
void hharray_print_f(HHArray array, void (*print)(void *));

/**
 * Prints the contents of the array by printing each value as a pointer.
 * @note `O(n)`
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
 * @note `O(n * log(n))` on average, `O(n²)` in worst case.
 */
void hharray_sort(HHArray array, int (*comparison)(const void *a, const void *b));

/**
 * Shuffles the array using a Fischer-Yates shuffle.
 * @pre assumes you have seeded the random number generator with `srand()`.
 * @note `O(n)`
 */
void hharray_shuffle(HHArray array);

/**
 * Returns a portion of the array's contents, from `start` to `end`, exclusive.
 * @return a new array with the contents of `array` from `start` to `end`.
 * @note if `start` or `end` are invalid indices, this function prints an error and exits.
 * @note if `start > end`, the slice will come back as if walked in reverse-order.
 * @note `O(n)` where `n` is `abs(end - start)`
 */
HHArray hharray_slice(HHArray array, size_t start, size_t end);

/**
 * Reverses an array in-place.
 * @note `O(n/2)`
 */
void hharray_reverse(HHArray array);

/**
 * @return `true` if the array is sorted as per the comparison function.
 * @note `O(n)`, but short circuits on finding an unsorted pair.
 */
int hharray_is_sorted(HHArray array, int (*comparison)(const void *a, const void *b));

/**
 * Swaps the values at the provided indices.
 * @note `O(1)`
 */
void hharray_swap(HHArray array, size_t first_index, size_t second_index);

#endif /* defined(__HHArray__HHArray__) */

