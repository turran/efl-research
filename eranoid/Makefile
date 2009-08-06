CFLAGS = -g `pkg-config --cflags eon` 
LIBS = `pkg-config --libs eon`

BINS  = eranoid
all: $(BINS)

eranoid_SRCS = \
main.c \
ball.c \
bar.c \
block.c \
level.c \
normalblock.c \
obstacle.c

eranoid_OBJS = $(eranoid_SRCS:.c=.o)
eranoid: $(eranoid_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(eranoid_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(BINS) *.o *~ *.n
