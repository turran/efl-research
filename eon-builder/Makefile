CFLAGS = -g `pkg-config --cflags eon`
LIBS = `pkg-config --libs eon`

BINS  = eon-builder
all: $(BINS)

SRCS = main.c rect.c circle.c scene.c shape.c control.c paint.c
OBJS = $(SRCS:.c=.o)
eon-builder: $(OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
