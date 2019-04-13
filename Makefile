CC=g++ -fopenmp
CFLAGS= -std=c++14 -O3 -Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-result
LDFLAGS=
EXEC=non-lin-search

all: $(EXEC)

non-lin-search: main.o mitm.o bool_op.o string_bool_op.o utils.o impl_info.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp mitm.h
	$(CC) -o $@ -c $< $(CFLAGS)

mitm.o: mitm.cpp mitm.h
	$(CC) -o $@ -c $< $(CFLAGS)

bool_op.o: bool_op.cpp mitm.h
	$(CC) -o $@ -c $< $(CFLAGS)

string_bool_op.o: string_bool_op.cpp mitm.h
	$(CC) -o $@ -c $< $(CFLAGS)

utils.o: utils.cpp mitm.h
	$(CC) -o $@ -c $< $(CFLAGS)

impl_info.o: impl_info.cpp mitm.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	rm test

check:
	gcc test.c -o test && ./test
