CFLAGS= -Wall -Werror -Wextra -Wpedantic -o3 -g

build_cache_akosik: src/test.c src/akosik/*
	gcc src/test.c src/akosik/cache.c src/akosik/lru.c $(CFLAGS) -o build/cache_kosik

test_akosik: build/cache_test
	build/cache_test
