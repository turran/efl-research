CFLAGS = -g `pkg-config --cflags eon gstreamer-0.10`
LIBS = `pkg-config --libs eon gstreamer-0.10`

BINS  = eon-gst
all: $(BINS)

SRCS = main.c
OBJS = $(SRCS:.c=.o)
eon-builder: $(OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
