#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "testing.h"
#include "cache.h"

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
    
    cache_t cache = create_cache(100 * sizeof(value), &our_modified_jenkins);
    cache_set(cache, key, value, sizeof(value));
    uint8_t *result = cache_get(cache, key, &val_size);
    test(result[0] == 10, "Can retrieve first entry.");
    test(val_size != 0, "cache_get sets val_size pointer");
}

int main(int argc, char *argv[])
{
    test_get_entry();
    printf("Testing...\n");
}
