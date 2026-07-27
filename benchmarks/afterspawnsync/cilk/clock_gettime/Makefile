.PHONY: serial

# Check the operating system
#ifeq ($(OS),Windows_NT)
#    DETECTED_OS := Windows
#else
#	DETECTED_OS := $(shell uname -s)
#endif

XCRUN=""
DETECTED_OS= $$(uname -s)
# if Mac, set xcrun
ifeq ($(DETECTED_OS),Darwin)
	XCRUN="xcrun ""
else
	XCRUN=""
endif

CC=		$(XCRUN)/opt/$$(ls /opt | grep "opencilk")/bin/clang # /opt/opencilk-2/bin/clang 
CFLAGS=		-fopencilk -O3 -Og -g #-fcilktool=cilkscale-benchmark #-lm # -fcilktool=cilkscale -lm -Wall -Wextra -Wconversion -std=c11 -Werror
INCFILES=   ../../../include/numthreads.c
SOURCE=		$(wildcard *.c)
TARGETS=	$(SOURCE:.c=)	

EXECS := $(foreach s,$(TARGETS),$(shell echo $(basename $(s)) | cut -c1-3))

all:
	@for s in $(SOURCE); do \
		name=$$(basename $$s); \
		exe=$$(echo $$name | cut -c1-3); \
		$(CC) $(CFLAGS) $(INCFILES) $$s -o $$exe -lm ; \
	done

build: $(TARGETS)

serial:
	$(MAKE) -C serial all

01N: 01N_spawntime_after_for_nosync_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 01N_spawntime_after_for_nosync_cilk.c -o 01N -lm 

01C: 01C_spawntime_after_for_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 01C_spawntime_after_for_cilk.c -o 01C -lm 

01D: 01D_spawntime_after_cilkfor_1_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 01D_spawntime_after_cilkfor_1_cilk.c -o 01D -lm 

01E: 01E_spawntime_after_one_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 01E_spawntime_after_one_cilk.c -o 01E -lm 

01F: 01F_spawntime_after_oneargs_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 01F_spawntime_after_oneargs_cilk.c -o 01F -lm 

02B: 02B_spawntime_before_cilkscope_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 02B_spawntime_before_cilkscope_cilk.c -o 02B -lm 

05A: 05A_spawntime_window_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 05A_spawntime_window_cilk.c -o 05A -lm 

04A: 04A_spawntime_beforefunc_sequential_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 04A_spawntime_beforefunc_sequential_cilk.c -o 04A -lm 

04B: 04B_spawntime_beforefunc_cilkscope_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 04B_spawntime_beforefunc_cilkscope_cilk.c -o 04B -lm 

04E: 04E_spawntime_beforefunc_one_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 04E_spawntime_beforefunc_one_cilk.c -o 04E -lm 

04F: 04F_spawntime_beforefunc_oneargs_cilk.c
	$(CC) $(CFLAGS) $(INCFILES) 04F_spawntime_beforefunc_oneargs_cilk.c -o 04F -lm 

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

7:
	@for s in $(wildcard 07*.c); do \
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

#clang  -fopenmp -lomp -O3 -Og -g omp/fib_omp.c -o ompfib
# 

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