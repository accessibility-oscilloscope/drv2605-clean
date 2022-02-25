CFLAGS := -g -Wall -Werror -pedantic --std=c99 -Wno-unused-const-variable -Wno-format
SRCS := *.c
HEADERS := *.h

all: haptic-driver

haptic-driver: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

format: $(SRCS) $(HEADERS)
	clang-format -i $(SRCS) $(HEADERS)

clean:
	rm -f haptic-driver

.phony: clean all
