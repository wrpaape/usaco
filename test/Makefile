CC   = gcc -Wall -O2
PROG = test

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(LIBS) -o $@ $^

clean:
	$(RM) $(PROG)
