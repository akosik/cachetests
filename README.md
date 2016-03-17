# Cache Testing

We have several different implementations of caches, all with the same API, in
our `src` directory. We will compile a set of unit tests to compare how robust
each implementation is. Tests are in `src/test.c`.

Each implementation has it's own command in the makefile to run the tests. To
run a test for `akosik`, for example, enter

```
make test_akosik
```

To build all tests for all caches, enter

```
make build_all
```

Similarly, to test all caches, enter

```
make test_all
```

And finally, to delete all test executables, enter

```
make clean_all
```
###Descriptions of tests (in Progress):

TGE ```test_get_entry()```:
Tests cache\_get for a single key, so obviously it also uses cache\_set to put the pair in the cache. This also checks if cache\_get returns a value size in the pointer that is passed to cache\_get. These tests need to be separated in the final set of tests.
  
SM ```set_multiple()```:
Tests cache_get for multiple keys.

TES ```test_empty_size()```:
Tests if the size of the cache (memsize) returns 0 right after creating it.

TS```test_size()```:
Tests if the cache updates the memory size when a key is inserted.

TSD ```test_size_after_delete()```:
Tests if the cache decrements its size once a value is removed.

EVIC ```eviction()```:
Tests if keys are evicted to make room for new keys. This only tests if the basic eviciton works i.e. a few keys are evicted. A test for scale will need to be added.

SS ```struct_set()```:
Tests if the cache can store more complex values such as structs.

GM ```get_modified()```:
Tests if the cache updates the value of a key i.e. a value is inserted with the same key as a value already in the table.

GN ```get_nonexistent()```:
Tests if cache\_get returns NULL when the key searched for is not in the cache.

RES ```resize_maybe()```:
Tests if the cache can resize without failure.  The name should be changed, as it has been confirmed that all caches will resize in this test.

VTB ```val_too_big()```:
Tests if the cache rejects but does not quit if a value is too big to be put in the cache.

VTBR ```val_too_big_but_replacing()```:
Tests if the cache takes into account the size of the value being replaced when determining whether a key-value pair needs to be evicted. To clarify, this happens when a value is being inserted with a key that already exists in the cache, like in get\_modified.

CMV ```cache_mallocing_vals()```:
Tests if the cache stores its own copy of each value and key instead of relying on the user not to free things/exit functions.
