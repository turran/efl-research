CFLAGS = -g `pkg-config --cflags ekeko` -lneko -pthread -fPIC
LIBS = `pkg-config --libs ekeko` -lneko

EONCFLAGS = -g `pkg-config --cflags eon`
EONLIBS = `pkg-config --libs eon`


BINS  = nekeko.ndll neon.ndll nekeko.n
all: $(BINS)

nekeko_ndll_SRCS = nekeko.c
nekeko_ndll_OBJS = $(nekeko_ndll_SRCS:.c=.o)
nekeko.ndll: $(nekeko_ndll_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) -shared $(nekeko_ndll_OBJS) -o $@

neon_ndll_SRCS = neon.c
neon_ndll_OBJS = $(neon_ndll_SRCS:.c=.o)
neon.ndll: $(neon_ndll_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(EONCFLAGS) $(EONLIBS) -shared $(neon_ndll_OBJS) -o $@

nekeko_n_SRCS = nekeko.neko
#nekeko_n_OBJS = $(nekeko_n_SRCS:.neko=.n)
nekeko.n: $(nekeko_n_SRCS)
	$(RM) $@
	nekoc $< 

.neko.n:
	nekoc $< $@
.c.o:
	$(CC) $(FLAGS) $(CFLAGS) $(EONCFLAGS) -c $< -o $@
clean:
	rm -rf $(BINS) *.o *~ *.n
