CC = clang
CFLAGS = -Wall -Wextra -pedantic -Isrc -Isrc/commons -Isrc/parser -Isrc/net -Isrc/frontier -Isrc/lookup
LIBXML_FLAGS = -lxml2
LIBCURL_FLAGS = -lcurl

COMMONS_SRCS := $(wildcard src/commons/*.c)
MAIN_SRCS    := $(wildcard src/*.c)
PARSER_SRCS  := $(wildcard src/parser/*.c)
NET_SRCS     := $(wildcard src/net/*.c)
FRNTR_SRCS   := $(wildcard src/frontier/*.c)
LOOKUP_SRCS   := $(wildcard src/lookup/*.c)

SRCS = $(COMMONS_SRCS) $(MAIN_SRCS) $(PARSER_SRCS) $(NET_SRCS) $(FRNTR_SRCS) $(LOOKUP_SRCS)
OBJS = $(SRCS:.c=.o)

FORMAT_FILES := $(shell find ./src \( -name "*.c" -o -name "*.h" \))

BUILD_DIR := build
TARGET := $(BUILD_DIR)/crawler

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBXML_FLAGS) $(LIBCURL_FLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

clean-objs:
	rm -f $(OBJS)

build: all clean-objs

format:
	clang-format -style="{BasedOnStyle: LLVM, IndentWidth: 4, TabWidth: 4, UseTab: Never}" -i $(FORMAT_FILES)

.PHONY: all clean clean-objs build format
