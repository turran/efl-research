CFLAGS = -g `pkg-config --cflags eina-0`
LIBS = `pkg-config --libs eina-0`

BINS  = buddy
all: $(BINS)

buddy_SRCS = buddy.c test.c
buddy_OBJS = $(buddy_SRCS:.c=.o)
buddy: $(buddy_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(buddy_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
