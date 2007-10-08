FLAGS = -g `pkg-config --cflags ecore_li ecore edata`
LIBS = `pkg-config --libs ecore_li ecore edata`

BINS  = ecore_li 
all: $(BINS)

ecore_li_SRCS = main.c
ecore_li_OBJS = $(ecore_li_SRCS:.c=.o)
ecore_li: $(ecore_li_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(ecore_li_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
