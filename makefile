CFLAGS= -Wall -Wextra -Wpedantic -o3 -g

build_all: build_akosik build_alasd build_aled build_apanana build_jmcosel build_ben build_zeplar

test_all: test_akosik test_alasd test_aled test_apanana test_jmcosel test_ben test_zeplar

clean_all:
	rm -r build/*

build/cache_akosik:
	gcc src/test.c src/testing.c src/akosik/cache.c src/akosik/lru.c $(CFLAGS) -o build/cache_akosik

test_akosik: build/cache_akosik
	./build/cache_akosik

clean_akosik:
	rm build/cache_akosik

build/cache_alasd: 
	gcc src/test.c src/testing.c src/alasd/cache.c src/alasd/LRU.c src/alasd/hash_table.c $(CFLAGS) -o build/cache_alasd

test_alasd: build/cache_alasd
	./build/cache_alasd

clean_alasd:
	rm build/cache_alasd

build/cache_aled:
	gcc src/test.c src/testing.c src/aled1027/cache.c src/aled1027/evict.c src/aled1027/node.c src/aled1027/dbLL.c $(CFLAGS) -o build/cache_aled

test_aled: build/cache_aled
	./build/cache_aled

clean_aled:
	rm build/cache_aled

build/cache_apanana:
	gcc src/test.c src/testing.c src/apanana/cache.c src/apanana/lru.c $(CFLAGS) -o build/cache_apanana

test_apanana: build/cache_apanana
	./build/cache_apanana

clean_apanana:
	rm build/cache_apanana

build/cache_jmcosel:
	gcc src/test.c src/testing.c src/jmcosel/cache.c $(CFLAGS) -o build/cache_jmcosel

test_jmcosel: build/cache_jmcosel
	./build/cache_jmcosel

clean_jmcosel:
	rm build/cache_jmcosel

build/cache_ben:
	gcc src/test.c src/testing.c src/weepingwillowben/hash_cache.c src/weepingwillowben/lru_replacement.c src/weepingwillowben/helper.c $(CFLAGS) -o build/cache_ben

test_ben: build/cache_ben
	./build/cache_ben

clean_ben:
	rm build/cache_ben

build/cache_zeplar:
	gcc src/test.c src/testing.c src/zeplar/cache.c src/zeplar/slabs.c src/zeplar/list.c src/zeplar/tools.c $(CFLAGS) -o build/cache_zeplar

test_zeplar: build/cache_zeplar
	./build/cache_zeplar

clean_zeplar:
	rm build/cache_zeplar
