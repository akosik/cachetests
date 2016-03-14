# Cache Testing

We have several different implementations of caches, all with the same API, in
our `src` directory. We will compile a set of unit tests to compare how robust
each implementation is. Tests are in `src/test.c`.

Each implementation has it's own command in the makefile to run the tests. To
run a test for `akosik`, for example, enter

```
make test_akosik
```
