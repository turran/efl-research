CFLAGS = -g `pkg-config --cflags ekeko`
#-I/home/jl/work/neuros/osd2/git/rootfs/fs/usr/include -I/home/jl/work/neuros/osd2/git/rootfs/fs/usr/include/eina-0/ -I/home/jl/work/neuros/osd2/git/rootfs/fs/usr/include/eina-0/eina
LIBS = `pkg-config --libs ekeko`

BINS  = ekeko
all: $(BINS)

ekeko_SRCS = main.c
ekeko_OBJS = $(ekeko_SRCS:.c=.o)
ekeko: $(ekeko_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(ekeko_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
