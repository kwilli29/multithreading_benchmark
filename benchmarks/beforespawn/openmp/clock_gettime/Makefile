.PHONY: serial 

MACBREW_GCC=""
# Check the operating system
#ifeq ($(OS),Windows_NT)
#    DETECTED_OS := Windows
#else
#    DETECTED_OS := $(shell uname -s)
#endif
DETECTED_OS := $(shell uname -s)
# if Mac, set gcc-14
ifeq ($(DETECTED_OS),Darwin)
	MACBREW_GCC:=-14
else
	MACBREW_GCC=""
endif

CC=		gcc$(MACBREW_GCC)
CFLAGS=		-fopenmp -Wall -O3 -Og -g # -lm
INCFILES=   ../../../include/numthreads.c
SOURCE=		$(wildcard *.c)
TARGETS=	$(SOURCE:.c=)	

EXECS := $(foreach s,$(TARGETS),$(shell echo $(basename $(s)) | cut -c1-3))

build: $(TARGETS)

06A: 06A_spawntree.openmp.c 
	$(CC) $(CFLAGS) $(INCFILES) 06A_spawntree.openmp.c  -o 06A -lm

1:
	@for s in $(wildcard 01*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done
2:
	@for s in $(wildcard 02*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done
3:
	@for s in $(wildcard 03*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done

4:
	@for s in $(wildcard 04*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done
5:
	@for s in $(wildcard 05*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done
6:
	@for s in $(wildcard 06*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done
8:
	@for s in $(wildcard 08*.c); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done
all:
	@for s in $(SOURCE); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm; \
	done

#clang  -fopenmp -lomp -O3 -Og -g omp/fib_omp.c -o ompfib

# For compilers to find libomp you may need to set:
#  export LDFLAGS="-L/opt/homebrew/opt/libomp/lib"
#  export CPPFLAGS="-I/opt/homebrew/opt/libomp/include"

#If you need to have llvm first in your PATH, run:
#  echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> /Users/Kelly/.bash_profile

#For compilers to find llvm you may need to set:
#  export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
#  export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"

clean:
	rm -f $(EXECS) *.s *.o *_.c lib*.so lib*.a
	rm -r *.dSYM
