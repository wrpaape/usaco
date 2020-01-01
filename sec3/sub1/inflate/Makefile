CC      = cc
CFLAGS  = -Wall -std=c11 -g -O0
LDFLAGS = -lm
PROG    = inflate

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -rf $(PROG) $(PROG).dSYM core
