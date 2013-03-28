CC=gcc
LIBS=-lSDL -lchipmunk
CFLAGS=-g -std=c99

# put objects in their own directory
SRCDIR  = src
OBJDIR  = obj
SRCS    = $(wildcard $(SRCDIR)/*.c)
_OBJS   = $(SRCS:src%=obj%)
OBJS    = $(_OBJS:%.c=%.o)

EXEC    = bin/swinging-game

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
