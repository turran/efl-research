CFLAGS = -g `pkg-config --cflags enesim` `pkg-config --cflags libpng`
LIBS = `pkg-config --libs enesim` `pkg-config --libs libpng`

BINS  = scanlines vector_pipeline1 vector_pipeline2
all: $(BINS)

sls_SRCS = scanlines.c png.c
sls_OBJS = $(sls_SRCS:.c=.o)
scanlines: $(sls_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(sls_OBJS) -o $@

vp1_SRCS = vector_pipeline1.c png.c surface.c
vp1_OBJS = $(vp1_SRCS:.c=.o)
vector_pipeline1: $(vp1_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(vp1_OBJS) -o $@

vp2_CFLAGS=`pkg-config --cflags exml` $(CFLAGS)
vp2_LIBS=`pkg-config --libs exml` $(LIBS) -lm
vp2_SRCS = vector_pipeline2.c png.c surface.c
vp2_OBJS = $(vp2_SRCS:.c=.o)
vector_pipeline2: $(vp2_OBJS)
	$(RM) $@
	$(CC) $(vp2_LIBS) $(vp2_CFLAGS) $(vp2_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
