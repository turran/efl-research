CFLAGS = -g `pkg-config --cflags etk2 sdl ecore`
LIBS = `pkg-config --libs etk2 sdl ecore`

BINS  = dummy_test
all: $(BINS)

dummy_test_SRCS = dummy_test.c \
dummy_canvas.c \
dummy_rect.c \
dummy.c \
dummy.h

dummy_test_OBJS = $(dummy_test_SRCS:.c=.o)
dummy_test: $(dummy_test_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(dummy_test_OBJS) -o $@
