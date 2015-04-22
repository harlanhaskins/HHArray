//
//  utilities.h
//
//  Created by Harlan Haskins on 4/4/15.
//  Copyright (c) 2015 harlanhaskins. All rights reserved.
//

#ifndef __utilities__
#define __utilities__

#include <stdio.h>

/// Runs `free_func` on all values in `list`, then frees `list`.
/// @param list A pointer to a void pointer of `size` size.
/// @param size The number of elements `list` should point to.
/// @param free_func A function pointer that frees an individual item in the list.
///                  If `free_func` is NULL, `freelist_f` defaults to `free`.
void freelist_f(void **list, size_t size, void (*free_func)(void *));

/// Frees all values in `list`, then frees `list`.
/// @param list A pointer to a void pointer of `size` size.
/// @param size The number of elements `list` should point to.
void freelist(void **list, size_t size);

/// Wrapper for calloc that perrors and exits if the
/// allocated memory comes back NULL.
void *hhcalloc(size_t count, size_t size);

/// Wrapper for realloc that perrors and exits if the
/// allocated memory comes back NULL.
void *hhrealloc(void *src, size_t size);

/// Calls `hhcalloc` with 1 as the count.
void *hhmalloc(size_t size);

#endif /* defined(__utilities__) */

