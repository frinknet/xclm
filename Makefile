include config.mk

HDR = xcbh.h
SRC = \
	window-current.c \
	window-root.c \
	window-focus.c \
	window-kill.c \
	window-size.c \
	window-position.c \
	window-geometry.c \
	window-move.c \
	window-resize.c \
	window-redraw.c \
	window-show.c \
	window-hide.c \
	window-toggle.c \
	window-manage.c \
	window-ignore.c \
	window-exists.c \
	window-command.c \
	window-class.c \
	window-name.c \
	window-raise.c \
	window-lower.c \
	window-parent.c \
	list-child-windows.c \
	list-hidden-windows.c \
	list-ignored-windows.c \
	list-managed-windows.c \
	list-visible-windows.c \
	is-window-ignored.c \
	is-window-managed.c \
	is-window-hidden.c \
	is-window-visible.c

OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)
MAN = $(SRC:.c=.1.gz)

.POSIX:
.SUFFIXES: .1 .1.gz

all: binutils manpages

binutils: $(BIN)

manpages:
	cd man; $(MAKE) $(MFLAGS)

$(OBJ): $(HDR) xcbh.o

.o:
	@echo "LD $@"
	@$(LD) $< xcbh.o -o $@ $(LDFLAGS)

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin/
	cd man; $(MAKE) $(MFLAGS) install

uninstall:
	@echo "uninstalling binaries"
	@for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/bin/$$util; \
	done
	cd man; $(MAKE) $(MFLAGS) uninstall

clean :
	rm -f $(OBJ) $(BIN) xcbh.o
	cd man; $(MAKE) $(MFLAGS) clean
