CFLAGS = `pkg-config --cflags etk` `pkg-config --cflags etch`
LIBS = `pkg-config --libs etk` `pkg-config --libs etch`

BINS  = etk-etch
all: $(BINS)

etk_etch_SRCS = main.c
etk_etch_OBJS = $(etk_etch_SRCS:.c=.o)
etk-etch: $(etk_etch_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(etk_etch_OBJS) -o $@

