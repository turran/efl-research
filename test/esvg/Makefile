CFLAGS = -g `pkg-config --cflags esvg sdl cairo`
LIBS = `pkg-config --libs esvg sdl cairo`

BINS  = esvg_test
all: $(BINS)

esvg_test_SRCS = main.c sdl_backend.c
esvg_test_OBJS = $(esvg_test_SRCS:.c=.o)
esvg_test: $(esvg_test_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(esvg_test_OBJS) -o $@

clean: 
	rm $(BINS)
	rm *.o
