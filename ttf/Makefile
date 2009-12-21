CFLAGS = -g `pkg-config --cflags enesim emage freetype2`
LIBS = `pkg-config --libs enesim emage freetype2`

BINS  = ttf ft
all: $(BINS)

ttf_SRCS = main.c ttf.c
ttf_OBJS = $(ttf_SRCS:.c=.o)
ttf: $(ttf_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(ttf_OBJS) -o $@

ft_SRCS = ft.c
ft_OBJS = $(ft_SRCS:.c=.o)
ft: $(ft_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(ft_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
