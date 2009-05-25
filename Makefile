PRG=answermethod
SRC=answermethod.c
OBJ=$(patsubst %.c,%.o,$(SRC))
PKG=dbus-1
CFLAGS+=-std=gnu99 -Wall -pedantic -I.
LDFLAGS+=-lm

# pkg-config dbus-1.pc is broken in openembeeded. Do it manually
CFLAGS+=`pkg-config --cflags $(PKG)`
LDFLAGS+=`pkg-config --libs $(PKG)`

all: $(PRG)

$(PRG): $(OBJ)

clean:
	rm -f $(PRG) *.o
