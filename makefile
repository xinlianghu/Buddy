CC = gcc
all:test
test:list.o buddy.o test.o
	$(CC) -m32 -g -o $@ $^
%.o:%.c
	$(CC) -m32 -g -c -o $@ $^
clean:
	rm -rf *.o
	rm -f test
	
