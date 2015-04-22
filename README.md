# HHArray

A very naïve resizable array in C.

**Don't use this. This is just fun. It's also horribly inefficient.**

It takes some design decisions from [CoreFoundation](http://en.wikipedia.org/wiki/Core_Foundation)
except without reference counting.

It's got some fun things, like `hharray_sort`, `hharray_shuffle`, and
built-ins for treating it like a stack and queue.

It's also got functional abstractions -- `hharray_map` and `hharray_filter`
that work on HHArrays.
