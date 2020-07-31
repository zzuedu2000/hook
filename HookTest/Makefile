CC = cc -m64
#CFLAGS = -g -fPIC
CFLAGS = -O2 -fPIC

all:test

test: HookTest.o
	$(CC) $(CFLAGS) $^ -o $@
	
clean:
	find ./ -name "*.o" -exec rm -f {} \;
	rm test;

