CC=g++
LIBS=-lSDL -lSDL_image -lBox2D
CFLAGS=-g

# put objects in their own directory
SRCDIR  = src
OBJDIR  = obj
SRCS    = $(wildcard $(SRCDIR)/*.cpp)
_OBJS   = $(SRCS:src%=obj%)
OBJS    = $(_OBJS:%.cpp=%.o)

EXEC    = bin/swinging-game

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
