#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "testing.h"
#include "cache.h"

//Initializes cache to maxmem of 255
cache_t init()
{
  cache_t cache = create_cache(255,NULL,NULL,NULL);
  return cache;
}

//Sets multiple values and checks if they are there
void set_multiple()
{
  cache_t cache = init();
  const uint8_t
    *key0 = "hello",
    *key1 = "thenumber3",
    *key2 = "goodbye",
    *key3 = "wow";
  uint8_t
    value0 = 1,
    value1 = 3;
  uint32_t value2 = 304;
  uint64_t value3 = 123123124;

  cache_set(cache,key0,&value0,sizeof(uint8_t));
  cache_set(cache,key1,&value1,sizeof(uint8_t));
  cache_set(cache,key2,&value2,sizeof(uint32_t));
  cache_set(cache,key3,&value3,sizeof(uint64_t));

  uint32_t val_size = 0;

  uint8_t
    val1 = *(uint8_t*)cache_get(cache,key0,&val_size),
    val2 = *(uint8_t*)cache_get(cache,key1,&val_size);
  uint32_t val3 = *(uint32_t*)cache_get(cache,key2,&val_size);
  uint64_t val4 = *(uint64_t*)cache_get(cache,key3,&val_size);

  test(val1 == 1 && val2 == 3 && val3 == 304 && val4 == 123123124, "cache_set stores (multiple) values that are accessible");

  destroy_cache(cache);
}

// Hash function borrowed from Alex Ledger's implementation
uint64_t our_modified_jenkins(key_type key)
{
    // https://en.wikipedia.org/wiki/Jenkins_hash_function
    uint32_t hash = *key;
    hash += (hash << 10);
    hash ^= (hash >> 6);
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return (uint64_t) hash;
}

//Checks if a custom hash at least doesn't crash the cache (since some didn't have this customization
void custom_hash()
{
    uint8_t key[2] = {'a', '\0'};
    uint8_t value[6] = {10,11,12,13,14,15};
    uint32_t val_size = 0;
    cache_t cache = create_cache(100 * sizeof(value), &our_modified_jenkins, NULL, NULL);

    cache_set(cache, key, value, sizeof(value));
    uint8_t *ret = (uint8_t*)cache_get(cache, key, &val_size);
    test(ret[0] == 10 && ret[1] == 11 && ret[2] == 12 && ret[3] == 13 && ret[4] == 14 && ret[5] == 15, "cache_get works when given a custom hash (doesn't have to use custom hash)");

    destroy_cache(cache);
}

//Tests if cache returns val_size from cache get
void test_get_entry()
{
    uint8_t key[2] = {'a', '\0'};
    uint8_t value[6] = {10,11,12,13,14,15};
    uint32_t val_size = 0;
    cache_t cache = create_cache(100 * sizeof(value), NULL, NULL, NULL);

    cache_set(cache, key, value, sizeof(value));
    cache_get(cache, key, &val_size);
    test(val_size != 0, "cache_get sets val_size pointer");

    destroy_cache(cache);
}

//cache has empty memsize used right after creation
void test_empty_size()
{
    cache_t cache = create_cache(1024, &our_modified_jenkins, NULL, NULL);
    uint64_t space = cache_space_used(cache);
    test(space == 0, "Empty cache uses zero space");

    destroy_cache(cache);
}

//cache updates memsize after cache_set
void test_size()
{
    cache_t cache = create_cache(1024, &our_modified_jenkins, NULL, NULL);
    uint8_t key[2] = {'a', '\0'};
    uint8_t value[6] = {10,11,12,13,14,15};
    uint32_t val_size = 0;
    cache_set(cache, key, value, sizeof(value));

    uint64_t space = cache_space_used(cache);
    test(space == sizeof(value), "Cache computes first value size correctly");

    destroy_cache(cache);
}

//cache decrements memsize used after a key is removed
void test_size_after_delete()
{
    cache_t cache = create_cache(1024, &our_modified_jenkins, NULL, NULL);
    uint8_t key[2] = {'a', '\0'};
    uint8_t value[6] = {10,11,12,13,14,15};
    uint32_t val_size = 0;
    cache_set(cache, key, value, sizeof(value));

    cache_delete(cache, key);

    uint64_t space = cache_space_used(cache);
    test(space == 0, "Cache size is zero after deleting all entries");

    destroy_cache(cache);
}

//cache evicts keys to make room for new ones (jmcosel has a memsize min of 64)
void eviction_couple()
{
  cache_t cache = create_cache(64,NULL,NULL,NULL);
  key_type
    key0 = "hello",
    key1 = "thenumber3",
    key2 = "goodbye",
    key3 = "wow";
  uint64_t
    value0 = 1,
    value1 = 3;
  uint8_t chararray[60];
  uint8_t value3 = 253;

  cache_set(cache,key0,&value0,sizeof(uint64_t));
  cache_set(cache,key1,&value1,sizeof(uint64_t));
  cache_set(cache,key2,chararray,60);
  cache_set(cache,key3,&value3,sizeof(uint8_t));

  uint32_t val_size = 0;
  uint8_t *val = (uint8_t*) cache_get(cache,key3,&val_size);
  test(*val == 253,"keys are evicted to make space for new values");
  destroy_cache(cache);
}

void eviction_LRU()
{
  cache_t cache = create_cache(10,NULL,NULL,NULL);
  key_type
    key0 = "hello",
    key1 = "thenumber3",
    key2 = "goodbye",
    key3 = "wow";
  uint8_t
    value0 = 1,
    value1 = 3;
  uint32_t value2 = 304;
  uint64_t value3 = 123123124;

  cache_set(cache,key0,&value0,sizeof(uint8_t));
  cache_set(cache,key1,&value1,sizeof(uint8_t));
  cache_set(cache,key2,&value2,sizeof(uint32_t));

  uint32_t val_size = 0;
  uint64_t *val;

  // access first input
  val = (uint64_t*) cache_get(cache,key0,&val_size);

  // Set something that will require an eviction
  cache_set(cache,key3,&value3,sizeof(uint64_t));

  // now get the last used value
  val = (uint64_t*) cache_get(cache,key1,&val_size);

  test(val == NULL,"Last accessed key is evicted");
  destroy_cache(cache);
}

//test struct for next test
struct test_struct
{
  uint8_t *word;
  uint32_t num;
  uint8_t *stuff;
};

//cache handles more complex val types
void struct_set()
{
  cache_t cache = init();
  key_type keystruct = "struct";
  struct test_struct value4 = { .word = "it's a bag of words!", .num = 42, .stuff = NULL};
  uint32_t *size = calloc(1,sizeof(uint32_t));

    cache_set(cache,keystruct,&value4,sizeof(struct test_struct));

    struct test_struct *holder = (struct test_struct*) cache_get(cache,keystruct,size);

    uint8_t *val5 = holder->word;
    uint32_t val6 = holder->num;
    uint8_t *val7 = holder->stuff;

    test(!strcmp(val5,"it's a bag of words!") && val6 == 42 && val7 == NULL,"cache stores structs appropriately");
    free(size);
    destroy_cache(cache);
}

//cache updates value if new key inserted is the same as one already in the cache
void get_modified()
{
  cache_t cache = init();
  key_type key = "hello";
  uint8_t val1 = 201;
  uint32_t val2 = 53;
  uint32_t *size = calloc(1,sizeof(uint32_t));

  cache_set(cache,key,&val1,sizeof(uint8_t));
  cache_set(cache,key,&val2,sizeof(uint32_t));
  uint32_t testval = *(uint32_t*)cache_get(cache,key,size);

  test(testval == 53,"cache updates values");
  free(size);
  destroy_cache(cache);
}

//cache returns NULL for key that isn't in the cache
void get_nonexistent()
{
  cache_t cache = init();
  key_type key = "sartre";
  uint32_t *size = calloc(1,sizeof(uint32_t));
  test(cache_get(cache,key,size) == NULL,"cache returns NULL for gets for keys not in cache");
  free(size);
  destroy_cache(cache);
}

//cache resizes properly (this does not check evict! (it took some time to isolate these tests) )
void resize()
{
  cache_t cache = create_cache(100000,NULL,NULL,NULL);
  uint64_t i = 1;
  uint8_t *key = malloc(100000);
  memset(key,0,10000);
  for(; i <= 10000; ++i)
    {
      strcat(key,"h");
      cache_set(cache,key,&i,sizeof(uint64_t));
    }
  uint32_t val_size = 0;
  uint64_t *val = (uint64_t*)cache_get(cache,key,&val_size);
  test(*val == 10000,"cache resizes without failure (initial table size checked, all should resize, none should evict)");
  free(key);
  destroy_cache(cache);
}

//cache does not insert values that are too big for the cache
void val_too_big()
{
  cache_t cache = create_cache(90,NULL,NULL,NULL);
  key_type key = "big";
  char large[100];
  cache_set(cache,key,large, 100);
  uint32_t val_size = 0;
  char *ret = (char*)cache_get(cache,key,&val_size);
  test(ret == NULL,"cache doesn't save values that are too big to fit in the user specified mem space");
  destroy_cache(cache);
}

//cache returns memused size == to 0 if the val being inserted is too big for the cache (the cache memsize == 0 if the cache didnt increase memsize
//because the value is too big for the cache) this test was ignored if val_too_big just segfaulted
void cache_does_not_change_maxmem()
{
  cache_t cache = create_cache(90,NULL,NULL,NULL);
  key_type key = "big";
  char large[91];
  cache_set(cache,key,large, 91);
  uint64_t memused = cache_space_used(cache);
  test(memused == 0,"cache does not change user specified maxmem bound");
  destroy_cache(cache);
}

//cache doesn't evict any keys if the new val is replacing one in the cache such that memsize is never exceeded
void val_too_big_but_replacing()
{
  cache_t cache = create_cache(82,NULL,NULL,NULL);
  key_type standin = "i shouldnt get removed";
  uint8_t standinval = 34;
  cache_set(cache,standin,&standinval,sizeof(uint8_t));
  key_type key = "doppleganger";
  uint8_t string[80];
  cache_set(cache,key,string,80);

  uint64_t new = 43;
  cache_set(cache,key,&new,sizeof(uint64_t));

  uint32_t val_size = 0;

  uint8_t *imstillhere = (uint8_t*) cache_get(cache,standin,&val_size);

  test(imstillhere != NULL,"cache doesnt evict if a value that would cause memory to exceed maxmem is replacing a value in such a way not to exceed maxmem");
  destroy_cache(cache);
}

//cache doesn't remove the old val if the new val is too big for the cache anyways
void val_too_big_and_replacing()
{
  cache_t cache = create_cache(89,NULL,NULL,NULL);
  key_type standin = "i shouldnt get removed";
  uint64_t standinval = 34;
  cache_set(cache,standin,&standinval,sizeof(uint64_t));
  key_type key = "doppleganger";
  uint8_t string[80] = "i shall remain";
  cache_set(cache,key,string,80);

  uint8_t new[90];
  cache_set(cache,key,&new,90);

  uint32_t val_size = 0;

  uint8_t *notreplaced = (uint8_t*) cache_get(cache,key,&val_size);

  test(!strcmp(notreplaced,"i shall remain"),"cache doesnt replace a value if the new value is too big for the cache");
  destroy_cache(cache);
}

//cache stores its own values instead of references passed in by the user
void cache_mallocing_vals()
{
  cache_t cache = init();
  key_type unique = "we are part of the collective, we have no name";
  uint8_t mutableval = 34;
  cache_set(cache,unique,&mutableval,sizeof(uint8_t));

  mutableval = 23;
  uint32_t val_size = 0;
  uint8_t *valnow = (uint8_t*)cache_get(cache,unique,&val_size);
  test(*valnow == 34,"cache malloc's values instead of storing references to (possible) local variables");
  destroy_cache(cache);
}

//cache handles large inserts
void cache_insert_huge()
{
  cache_t cache = create_cache(512,NULL,NULL,NULL);
  key_type huge = "large";
  uint8_t *largeval = malloc(51);
  cache_set(cache,huge,largeval,51);
  uint32_t val_size = 0;
  uint8_t *retval = (uint8_t*)cache_get(cache,huge,&val_size);
  test(retval != NULL,"cache handles inserts of huge values");
}

int main(int argc, char *argv[])
{
  cache_insert_huge();
  test_get_entry();
  set_multiple();
  test_empty_size();
  test_size();
  test_size_after_delete();
  eviction_couple();
  struct_set();
  get_modified();
  get_nonexistent();
  resize();
  val_too_big();
  val_too_big_but_replacing();
  cache_mallocing_vals();
  val_too_big_and_replacing();
  cache_does_not_change_maxmem();
  custom_hash();
  eviction_LRU();
}
