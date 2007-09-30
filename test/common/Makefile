FLAGS = -g
LIBS = 

BINS  = test_common.a
all: $(BINS)

test_common_SRCS = time.c
test_common_OBJS = $(test_common_SRCS:.c=.o)
test_common.a: $(test_common_OBJS)
	$(RM) $@
	$(CC) $(LIBS) $(CFLAGS) $(LDFLAGS) $(test_common_OBJS) -o $@

.c.o:
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@
clean::
	rm -rf $(BINS) *.o *~
