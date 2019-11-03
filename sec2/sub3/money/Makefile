CC     = cc
CFLAGS = -Wall -std=c11 -g
PROG   = money

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(PROG) $(PROG).dSYM
