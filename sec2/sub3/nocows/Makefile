CC   = cc -Wall -std=c11 -g
PROG = nocows

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) -o $@ $^

clean:
	$(RM) -rf $(PROG) $(PROG).dSYM
