
CC = gcc
PKGCONFIG = pkg-config
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-2.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-2.0)

SRC = $(wildcard src/*.c)
TARGET = integrated_app

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -rdynamic -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET)
