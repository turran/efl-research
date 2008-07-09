CFLAGS = -g `pkg-config --cflags evas ecore`
LIBS = `pkg-config --libs evas ecore`

BINS  = ee_test lib.so
all: $(BINS)

ee_test_SRCS = main.c
ee_test_OBJS = $(ee_test_SRCS:.c=.o)
ee_test: $(ee_test_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(ee_test_OBJS) -o $@


lib.so: lib.c
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) -shared lib.c -o $@

clean: 
	rm $(BINS)
	rm *.o
