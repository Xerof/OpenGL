all: main
clean:
		rm -f out *.o
.PHONY: all clean

CC=g++
packages = glfw3 glew
CFLAGS := $(shell pkg-config --cflags $(packages))
LIBS := $(shell pkg-config --static --libs $(packages))

main: main.o
		$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

