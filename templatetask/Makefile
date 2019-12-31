CC      = cc
CFLAGS  = -Wall -std=c11 -g
LDFLAGS = -lm
PROG    = templatetask

all: $(PROG)

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -rf $(PROG) $(PROG).dSYM core
