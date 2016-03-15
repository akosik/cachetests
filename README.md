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

