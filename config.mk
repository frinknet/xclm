PREFIX:=/usr
MANPREFIX:=$(PREFIX)/share/man

CC      := cc
LD      := $(CC)
GZ      := gzip
ST      := strip

CFLAGS  += -std=c99 -pedantic -Wall -Os -s
LDFLAGS += -lxcb
STFLAGS += -s

