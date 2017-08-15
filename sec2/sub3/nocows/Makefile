CC   = gcc -Wall -O2 -std=c11 -g
PROG = nocows

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) -o $@ $^

clean:
	$(RM) $(PROG)
