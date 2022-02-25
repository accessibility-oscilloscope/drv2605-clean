CFLAGS := -g -Wall -Werror -pedantic --std=c99 -Wno-unused-const-variable -Wno-format
SRCS := *.c
HEADERS := *.h

all: haptic-driver

haptic-driver: $(SRCS)
	$(CC) $(CFLAGS) -o driver $(SRCS)

format: $(SRCS) $(HEADERS)
	clang-format -i $(SRCS) $(HEADERS)

clean:
	rm -f driver

.phony: clean all
