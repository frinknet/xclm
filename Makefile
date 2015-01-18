include config.mk

HDR = xcbh.h
SRC = \
	current-root.c \
	current-window.c \
	window-focus.c \
	window-kill.c \
	window-move.c \
	window-resize.c \
	window-warp.c \
	window-moveby.c \
	window-resizeby.c \
	window-warpby.c \
	window-show.c \
	window-hide.c \
	window-toggle.c \
	window-manage.c \
	window-ignore.c \
	window-raise.c \
	window-lower.c \
	window-get-size.c \
	window-get-position.c \
	window-get-geometry.c \
	window-get-command.c \
	window-get-class.c \
	window-get-name.c \
	window-get-parent.c \
	window-get-root.c \
	window-list-ignored.c \
	window-list-managed.c \
	window-list-visible.c \
	window-list-hidden.c \
	window-list-children.c \
	window-list-parents.c \
	window-is-ignored.c \
	window-is-managed.c \
	window-is-hidden.c \
	window-is-visible.c \
	window-is-class.c \
	window-is-command.c \
	window-is-valid.c

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
