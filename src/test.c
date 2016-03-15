#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "testing.h"
#include "cache.h"

cache_t checkInit()
{
  cache_t cache = create_cache(255,NULL,NULL,NULL);
  return cache;
}

int checkSet(cache_t cache)
{
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

  test(val1 == 1 && val2 == 3 && val3 == 304 && val4 == 123123124, "cache_set stores values that are accessible");
  return 1;
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
    free(cache);
}

int main(int argc, char *argv[])
{
    test_get_entry();
    cache_t cache = checkInit();
    checkSet(cache);
}
