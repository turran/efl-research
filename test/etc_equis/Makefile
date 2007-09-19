FLAGS = `pkg-config --cflags etc` `pkg-config --cflags equis`
LIBS = `pkg-config --libs etc` `pkg-config --libs equis`

BINS  = vector_pipeline1 
all: $(BINS)

vector_pipeline1_SRCS = vector_pipeline1.c
vector_pipeline1_OBJS = $(vector_pipeline1_SRCS:.c=.o)
vector_pipeline1: $(vector_pipeline1_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(vector_pipeline1_OBJS) -o $@

############################################################################
#### boilerplate
.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
