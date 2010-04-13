CFLAGS = -g `pkg-config --cflags eon ecore-ipc ecore`
LIBS = `pkg-config --libs eon ecore-ipc ecore`

BINS  = eon-server
all: $(BINS)

SRCS = main.c
OBJS = $(SRCS:.c=.o)
eon-server: $(OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
