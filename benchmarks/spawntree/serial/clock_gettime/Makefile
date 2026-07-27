CC=		gcc 
CFLAGS=		-O3 -Og -g # -Wall -Wconversion #-lm
INCFILES=   ../../../include/numthreads.c
SOURCE=		$(wildcard *.c)
TARGETS=	$(SOURCE:.c=)	

EXECS := $(foreach s,$(TARGETS),$(shell echo $(basename $(s)) | cut -c1-3))

build: $(TARGETS)

01A: 01A_spawntime_after_sequential_serial.c
	$(CC) $(CFLAGS) $(INCFILES) 01A_spawntime_after_sequential_serial.c -o 01A -lm

01C: 01C_spawntime_after_for_serial.c
	$(CC) $(CFLAGS) $(INCFILES) 01C_spawntime_after_for_serial.c -o 01C -lm

01D: 01D_spawntime_after_cilkfor_1_serial.c
	$(CC) $(CFLAGS) $(INCFILES) 01D_spawntime_after_cilkfor_1_serial.c -o 01D -lm

05A: 05A_spawntime_window_serial.c
	$(CC) $(CFLAGS) $(INCFILES) 05A_spawntime_window_serial.c -o 05A -lm

04B: 04B_spawntime_beforefunc_cilkscope_serial.c
	$(CC) $(CFLAGS) $(INCFILES) 04B_spawntime_beforefunc_cilkscope_serial.c -o 04B -lm

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
all:
	@for s in $(SOURCE); do \
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
