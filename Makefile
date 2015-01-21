include config.mk

HDR = xcbtools.h
SRC = \
	create-window.c \
	current-root.c \
	current-window.c \
	event-watch.c \
	event-trigger.c \
	event-test.c \
	window-focus.c \
	window-kill.c \
	window-move-by.c \
	window-move-to.c \
	window-warp-by.c \
	window-warp-to.c \
	window-resize-by.c \
	window-resize-to.c \
	window-show.c \
	window-hide.c \
	window-toggle.c \
	window-border.c \
	window-manage.c \
	window-ignore.c \
	window-raise.c \
	window-lower.c \
	window-reparent.c \
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

all: binutils

binutils: $(BIN)

manpages:
	cd man; $(MAKE) $(MFLAGS)

$(OBJ): $(HDR) xcbtools.o

.o:
	@echo "LD $@"
	@$(LD) $< xcbtools.o -o $@ $(LDFLAGS)
	@echo "ST $@"
	@$(ST) $(STFLAGS) $@

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/share/xcbtools/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/share/xcbtools/
	cp -rf events $(DESTDIR)$(PREFIX)/share/xcbtools/
	cp -f elegacy-session $(DESTDIR)$(PREFIX)/share/xcbtools/
	cp -f elegacy.desktop $(DESTDIR)$(PREFIX)/share/xsessions/

uninstall:
	@echo "uninstalling binaries"
	@for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/share/xcbtools/$$util; \
	done
	rm -rf $(DESTDIR)$(PREFIX)/share/xcbtools/events
	rm -f $(DESTDIR)$(PREFIX)/share/xcbtools/elegacy-session
	rm -f $(DESTDIR)$(PREFIX)/share/xsessions/elegacy.desktop

clean:
	rm -f $(OBJ) $(BIN) xcbtools.o

manclean:
	cd man; $(MAKE) $(MFLAGS) clean
