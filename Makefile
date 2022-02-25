CFLAGS := -g -Wall -Werror -pedantic --std=c99
SRCS := *.c
HEADERS := *.h

all: driver

driver: $(SRCS)
	gcc $(CFLAGS) -o driver $(SRCS)

format: $(SRCS) $(HEADERS)
	clang-format -i $(SRCS) $(HEADERS)

clean:
	rm -f driver

.phony: clean all
