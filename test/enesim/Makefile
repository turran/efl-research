FLAGS = -g `pkg-config --cflags enesim`
LIBS = `pkg-config --libs enesim`

BINS  = vector_pipeline1 
all: $(BINS)

vector_pipeline1_SRCS = vector_pipeline1.c
vector_pipeline1_OBJS = $(vector_pipeline1_SRCS:.c=.o)
vector_pipeline1: $(vector_pipeline1_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(vector_pipeline1_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
