# Define variables
CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
TARGET = main
SOURCE = main.c

# Default target to build the binary
all: $(TARGET)

# Rule to build the binary
$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

# Rule to build and run the binary
run: $(TARGET)
	./$(TARGET)

# Rule to clean the build
clean:
	rm -f $(TARGET)

.PHONY: all run clean
