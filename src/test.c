#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "testing.h"
#include "cache.h"

void test_get_entry()
{
    cache_t cache = create_cache(256);
    uint8_t key = 20;
    uint64_t value = 420;
    uint32_t val_size = sizeof(value);
    cache_set(cache, &key, &value, &val_size);
    uint64_t result = cache_get(cache, &key, &val_size);
    test(result == 420, "Can retrieve first entry.");
}

int main(int argc, char *argv[])
{
    test_get_entry();
    printf("Testing...\n");
}
