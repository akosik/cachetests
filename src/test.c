#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "testing.h"
#include "cache.h"

cache_t init()
{
  cache_t cache = create_cache(255,NULL,NULL,NULL);
  return cache;
}

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

void test_get_entry()
{
    uint8_t key[2] = {'a', '\0'};
    uint8_t value[6] = {10,11,12,13,14,15};
    uint32_t val_size = 0;
    cache_t cache = create_cache(100 * sizeof(value), &our_modified_jenkins, NULL, NULL);

    cache_set(cache, key, value, sizeof(value));
    uint8_t *result = cache_get(cache, key, &val_size);
    test(result[0] == 10, "Can retrieve first entry.");
    test(val_size != 0, "cache_get sets val_size pointer");

    destroy_cache(cache);
}

void test_empty_size()
{
    cache_t cache = create_cache(1024, &our_modified_jenkins, NULL, NULL);
    uint64_t space = cache_space_used(cache);
    test(space == 0, "Empty cache uses zero space");

    destroy_cache(cache);
}

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

cache_t init_tiny()
{
  cache_t cache = create_cache(10,NULL,NULL,NULL);
  return cache;
}

void eviction()
{
  cache_t cache = init_tiny();
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
  cache_set(cache,key3,&value3,sizeof(uint64_t));

  uint32_t val_size = 0;
  uint64_t *val;

  val = (uint64_t*) cache_get(cache,key3,&val_size);

  test(*val == 123123124,"keys are evicted to make space for new values");
  destroy_cache(cache);
}

struct test_struct
{
  uint8_t *word;
  uint32_t num;
  uint8_t *stuff;
};

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

void get_nonexistent()
{
  cache_t cache = init();
  key_type key = "sartre";
  uint32_t *size = calloc(1,sizeof(uint32_t));
  test(cache_get(cache,key,size) == NULL,"cache returns NULL for gets for keys not in cache");
  free(size);
  destroy_cache(cache);
}

void resize_maybe()
{
  cache_t cache = init();
  uint64_t i = 0;
  uint8_t *key = malloc(10000);
  for(; i <= 1000; ++i)
    {
      strcat(key, "h");
      cache_set(cache,key,&i,sizeof(uint8_t));
    }
  free(key);
  //destroy_cache(cache);
  test(true,"cache resizes (most likely) without failure");
}

void val_too_big()
{
  cache_t cache = init_tiny();
  key_type key = "big";
  char *large = "this value is far too big for this tiny cache made by init_tiny";
  cache_set(cache,key,large, strlen(large) + 1);
  test(true,"if there's no error message on the line above, or the program halted, there's probably a bug in handling values that are too big for the cache");
  free(cache);
}

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

}

int main(int argc, char *argv[])
{
  test_get_entry();
  set_multiple();
  test_empty_size();
  test_size();
  test_size_after_delete();
  eviction();
  struct_set();
  get_modified();
  get_nonexistent();
  resize_maybe();
  //test_overflow();
  val_too_big();
  val_too_big_but_replacing();
}
