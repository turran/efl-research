CFLAGS = -g `pkg-config --cflags esvg`
LIBS = `pkg-config --libs esvg`

BINS  = esvg_test
all: $(BINS)

esvg_test_SRCS = main.c
esvg_test_OBJS = $(esvg_test_SRCS:.c=.o)
esvg_test: $(esvg_test_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(esvg_test_OBJS) -o $@

