CC = arm-linux-gnueabihf-gcc
CXX = g++
CFLAGS = -std=c11 -Wall -static
CXXFLAGS = -std=c++11 -O3 -Wall -g

TGT = shell_sort gemm binary_search spfa
SRC = $(addsuffix .c, $(TGT))
INPUT = $(addsuffix .in, $(TGT))
GEN = $(addsuffix _gen, $(TGT))
GEN_SRC = $(addsuffix .cc, $(GEN))

shell_sort_args = 1000000
gemm_args = 1024 1024 1024 
hash_args = 60000
binary_search_args = 4000000 4000000
spfa_args = 1500000 10000000
lz77_args = 1000000

all: $(TGT) hello

$(TGT): $(SRC) $(GEN_SRC)
	$(CXX) $(CXXFLAGS) $@_gen.cc -o $@_gen
	./$@_gen $@.in $(shell echo $($@_args))
	$(CC) $(CFLAGS) $@.c -o $@

hello: hello.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TGT) $(GEN) $(INPUT) hello

.PHONY: all clean
