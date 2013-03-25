CC=g++
LIBS=-lSDL -lSDL_image -lBox2D
CFLAGS=-g

# put objects in their own directory
SRCDIR  = src
OBJDIR  = obj
SRCS    = $(wildcard $(SRCDIR)/*.cpp)
_OBJS   = $(SRCS:src%=obj%)
OBJS    = $(_OBJS:%.cpp=%.o)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o main
