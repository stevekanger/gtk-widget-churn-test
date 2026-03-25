CC = gcc
PKGS = gtk4 gtk4-layer-shell-0
CFLAGS = $(shell pkg-config --cflags $(PKGS)) -Wall -Wextra -std=c99 -g
LIBS = $(shell pkg-config --libs $(PKGS))
SRC_DIR = src
BUILD_DIR = build
CACHE_DIR = .cache
EXECUTABLE_FILE_NAME = gtk-widget-churn
EXECUTABLE = $(BUILD_DIR)/$(EXECUTABLE_FILE_NAME)
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
HEADERS = $(shell find $(SRC_DIR) -name '*.h')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

.PHONY: all run valgrind debug clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(EXECUTABLE)
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full \
	         --track-origins=yes \
	         --show-leak-kinds=definite \
	         --errors-for-leak-kinds=definite \
	         --quiet \
	         ./$(EXECUTABLE)

debug:
	coredumpctl gdb

clean:
	rm -rf $(BUILD_DIR) $(CACHE_DIR)
