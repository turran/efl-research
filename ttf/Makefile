CFLAGS = -g `pkg-config --cflags enesim`
LIBS = `pkg-config --libs enesim`

BINS  = ttf
all: $(BINS)

ttf_SRCS = main.c ttf.c ttf_cmap.c ttf_interp.c
ttf_OBJS = $(ttf_SRCS:.c=.o)
ttf: $(ttf_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(ttf_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
