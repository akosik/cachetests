#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cache.h"

cache_t checkInit()
{
  cache_t cache = create_cache(255,NULL,NULL,NULL);
  printf("Test 0: Success!\n");
  return cache;
}

int checkSet(cache_t cache)
{
  const uint8_t*
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
  printf("Hi\n");
  uint8_t
    val1 = *(uint8_t*)cache_get(cache,key0,&val_size),
    val2 = *(uint8_t*)cache_get(cache,key1,&val_size);
  uint32_t val3 = *(uint32_t*)cache_get(cache,key2,&val_size);
  uint64_t val4 = *(uint64_t*)cache_get(cache,key3,&val_size);

  assert(val1 == 1);
  assert(val2 == 3);
  assert(val3 == 304);
  assert(val4 == 123123124);

  printf("Test 1: Success!\n");
  return 1;
}

int main(int argc, char *argv[])
{
  cache_t cache = checkInit();
  checkSet(cache);
}
