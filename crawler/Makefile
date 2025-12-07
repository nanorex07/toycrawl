CC = clang
CFLAGS = -Wall -Wextra -pedantic -I../commons -Isrc -Isrc/parser

COMMONS_SRCS := $(wildcard ../commons/*.c)
MAIN_SRCS := $(wildcard src/*.c)
PARSER_SRCS := $(wildcard src/parser/*.c)
SRCS = $(COMMONS_SRCS) $(MAIN_SRCS) $(PARSER_SRCS)
OBJS = $(SRCS:.c=.o)

TARGET = crawler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

clean-objs:
	rm -f $(OBJS)

build: all clean-objs

.PHONY: all clean clean-objs build-clean
