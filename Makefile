COMPILE_FOLDERS := src
TARGET := RobloxBootleg
CC := gcc
CFLAGS := -Wall -Wextra -O3 -Iinclude
ifeq ($(OS),Windows_NT)
	LDFLAGS := -lgdi32 -lopengl32 -lglfw3
else
	LDFLAGS := -lGL -lX11 -lXrandr -lXi -ldl -lpthread -lm -lglfw
endif

SOURCES := $(wildcard $(addsuffix /*.c,$(COMPILE_FOLDERS)))
OBJECTS := $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
