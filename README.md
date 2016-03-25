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
###Descriptions of tests:

ValSize ```test_get_valsize()```:
Checks if `cache_get` inserts the value size into val_size.

SM ```set_multiple()```:
Tests cache_get for multiple keys.

TES ```test_empty_size()```:
Tests if the size of the cache (memsize) returns 0 right after creating it.

TS```test_size()```:
Tests if the cache updates the memory size when a key is inserted.

TSD ```test_size_after_delete()```:
Tests if the cache decrements its size once a value is removed.

EVIC ```eviction_couple()```:
Tests if keys are evicted to make room for new keys. This only tests if the basic eviciton works i.e. a few keys are evicted. A test for scale will need to be added.

EAG ```evict_after_get()```:
Tests if cache evicts the last used value. Inputs a few values, access first cached value, adds a value that will cause an eviction, and then checks that the second added value gets evicted, the first value added remains, and the thrid value added is removed.

SS ```struct_set()```:
Tests if the cache can store more complex values such as structs.

GM ```get_modified()```:
Tests if the cache updates the value of a key i.e. a value is inserted with the same key as a value already in the table.

GN ```get_nonexistent()```:
Tests if cache\_get returns NULL when the key searched for is not in the cache.

RES ```resize()```:
Tests if the cache can resize without failure.

VTB ```val_too_big()```:
Tests if the cache rejects but does not quit if a value is too big to be put in the cache.

VTBR ```val_too_big_but_replacing()```:
Tests if the cache takes into account the size of the value being replaced when determining whether a key-value pair needs to be evicted. To clarify, this happens when a value is being inserted with a key that already exists in the cache, like in get\_modified.

CMV ```cache_mallocing_vals()```:
Tests if the cache stores its own copy of each value and key instead of relying on the user not to free things/exit functions.

VTAR ```val_too_big_and_replacing```:
Checks if the cache removes a value that is about to be replaced even though the value being inserted is too big for the cache.  We say the test fails if the old value is removed but this test seems to be implementation dependent (we may or may not wnat to keep old values in the cache.  if we are following the memcache paper designed we do want the cache to delete the old value.).  We thought it was good to include anyways.

SMEM ```cache_does_not_change_maxmem()```:
Tests if the cache modifies the maxmem value after the user sets it.

CIH ```cache_insert_huge```:
Checks if the cache can insert larger values into the cache.

CRBP ```cache_returns_bad_pointers```:
Checks if the cache makes a copy of the value to return to the client in `cache_get` (as opposed to giving a pointer to the 
value in the cache.

CH ```custom_hash```:
Tests if the custom_hash function works for caches that implemented it.

## Test Results

Test | Group 1 | Group 2 | Group 3 | Group 4 | Group 5 | Group 6 | Group 7 |
-----|---------|---------|---------|---------|---------|---------|---------|
ValSize  |  :+1:   |  :+1:   |   :x:   |   :x:  |  :+1:   |   :+1:  |  :+1:   |
SM   |  :+1:   |  :+1:   |   :x:   |   :+1:   |  :+1:   |   :x:   |  :+1:   |
TES  |  :+1:   |  :+1:   |   :+1:  |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
TS   |  :+1:   |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
TSD  |  :+1:   |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
EVIC |  :+1:   |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :x:  |  :+1:   |
EAG  |  :x:    |  :x:    |   :x:   |   :x:  |  :x:   |   :x:   |  :+1:   |
SS   |  :+1:   |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
GM   |  :+1:   |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
GN   |  :+1:   |  :x:    |   :+1:  |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
RES  |  :x:    |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
VTB  |  :x:    |  :x:    |   :x:   |   :+1:  |  :+1:   |   :x:   |  :+1:   |
VTBR |  :x:    |  :x:    |   :x:   |   :x:   |  :x:    |   :+1:  |  :+1:   |
CMV  |  :+1:   |  :+1:   |   :x:   |   :+1:  |  :+1:   |   :x:   |  :+1:   |
VTAR |  :x:    |  :x:    |   :x:   |   :+1:  |  :+1:   |   :x:   |  :x:    |
SMEM |  :x:    |  :+1:   |   :+1:  |   :+1:  |  :+1:   |   :x:  |  :+1:   |
CIH |  :+1:    |  :+1:   |   :x:  |   :+1:  |  :+1:   |   :+1:  |  :+1:   |
CRBP |  :x:    |  :+1:   |   :x:  |   :x:  |  :x:   |   :x:  |  :x:   |
CH |  :+1:    |  :+1:   |   :x:  |   :+1:  |  :+1:   |   :+1:  |  :+1:   |

### Group 1 Issues
Had a problem with resize and the code modified the max size of the cache after the user specified it.

### Group 2 Issues
The LRU test triggers a failed assertion in function `evict_get`. The same error is triggered with the GN test. It seems the cache fails when asked to get something not in the cache. That's a serious bug!  Most of the bugs come from the assertions, which, according to the 
api, should've been handled differently.

### Group 3 Issues
Group 3's program often fails with complaints about not being able to allocate any slabs. This is partly because we are testing with small cache sizes.  Most of the bugs we reported for this cach emay not be actually from the bug we were testing for but we could not get this cache working properly.

### Group 4 Issues
Did not implement val_size in `cache_get` and did not check if a value was replacing another before checking if the memory of the cache was full.  In `cache_get`: Did not return a pointer to a value separate from the one in the cache.  Also failed to properly update the eviction policy in `cache_get`.

### Group 5 Issues
Alec's cache: it was enlightening to work on my own cache because I had already known there was a bug in my code the day after submitting the last homework.  However, I could not tease it out in an isolated test (without inadvertently testing resize) because the bug only occured a very small portion of the time and requires very specific inserts. This cache also had issues with returning a pointer to an object in the cache when returning from `cache_get` and failing to update the eviction policy properly in `cache_get`.

### Group 6 Issues
The EAG test triggers an invalid read in `link_list_top` causing a segmentation fault. Does not copy values into the cache but only stores references to user-owned values.  This made it hard to keep track of other bugs.

### Group 7 Issues
Had a common problem with not handing the user a pointer to their own value instead of the one in the cache.

##Final Comments
TDD turned out to be harder to follow in practice because some caches had ubiquitous bugs, such as not malloc'ing values into the cache from group 6.  I also realized while testing my own cache that there are drawbacks to restricting oneself to one bug check per test since I could not devise a test that exposed a bug I knew I had (but couldn't test without also resizing the cache).  However, tdd has helped us find a panoply of bugs in our caches and has thus proven its usefulness, if not its perfection.
