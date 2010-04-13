CFLAGS = -g `pkg-config --cflags eon gstreamer-0.10`
LIBS = `pkg-config --libs eon gstreamer-0.10`

BINS  = eon-gst testapi
all: $(BINS)

eon_gst_SRCS = main.c
eon_gst_OBJS = $(eon_gst_SRCS:.c=.o)
eon-gst: $(eon_gst_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(eon_gst_OBJS) -o $@

testapi_SRCS = testapi.c
testapi_OBJS = $(testapi_SRCS:.c=.o)
testapi: $(testapi_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(testapi_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
