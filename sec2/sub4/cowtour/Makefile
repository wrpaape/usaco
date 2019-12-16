CC      = cc
CFLAGS  = -Wall -std=c11 -g
LDFLAGS = -lm
PROG    = cowtour

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(PROG) $(PROG).dSYM core
