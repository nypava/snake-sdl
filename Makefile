
CC = gcc
COMMON_CFLAGS = -Wall -Wextra -std=c99 -pedantic
CFLAGS += $(shell pkg-config --cflags sdl3) $(COMMON_CFLAGS) -MMD -MP
COMMON_LIBS = -lm
LIBS = $(shell pkg-config --libs sdl3) $(COMMON_LIBS) -L/usr/lib -lSDL3_ttf

SRC = main.c snake.c food.c obstacle.c
BUILD_DIR = build
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

TARGET = snake

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

-include $(OBJS:.o=.d)
