CFLAGS = -g `pkg-config --cflags esvg` `pkg-config --cflags sdl`
LIBS = `pkg-config --libs esvg` `pkg-config --libs sdl`

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
