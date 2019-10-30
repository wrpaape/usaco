CC     = cc
CFLAGS = -Wall -std=c11 -g
PROG   = zerosum

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(PROG) $(PROG).dSYM
