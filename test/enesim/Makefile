FLAGS = -g `pkg-config --cflags enesim` `pkg-config --cflags libpng`
LIBS = `pkg-config --libs enesim` `pkg-config --libs libpng`

BINS  = vector_pipeline1 
all: $(BINS)

vector_pipeline1_SRCS = vector_pipeline1.c png.c surface.c
vector_pipeline1_OBJS = $(vector_pipeline1_SRCS:.c=.o)
vector_pipeline1: $(vector_pipeline1_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(vector_pipeline1_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
