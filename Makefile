include config.mk

HDR = xmpl.h xcmd.h xclm.h
SRC = \
	current-root.c \
	current-window.c \
	event-watch.c \
	event-trigger.c \
	event-test.c \
	window-create.c \
	window-focus.c \
	window-kill.c \
	window-nudge.c \
	window-move.c \
	window-morph.c \
	window-warp.c \
	window-grow.c \
	window-shrink.c \
	window-resize.c \
	window-show.c \
	window-hide.c \
	window-toggle.c \
	window-border.c \
	window-background.c \
	window-manage.c \
	window-ignore.c \
	window-raise.c \
	window-lower.c \
	window-reparent.c \
	window-size.c \
	window-position.c \
	window-geometry.c \
	window-command.c \
	window-class.c \
	window-name.c \
	window-parent.c \
	window-get.c \
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

$(OBJ): $(HDR) xmpl.o

.o:
	@echo "LD $@"
	@$(LD) $< xmpl.o -o $@ $(LDFLAGS)
	@echo "ST $@"
	@$(ST) $(STFLAGS) $@

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/share/xmpl/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/share/xmpl/
	cp -rf events $(DESTDIR)$(PREFIX)/share/xmpl/
	cp -f elegacy-session $(DESTDIR)$(PREFIX)/share/xmpl/
	cp -f elegacy.desktop $(DESTDIR)$(PREFIX)/share/xsessions/

uninstall:
	@echo "uninstalling binaries"
	@for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/share/xmpl/$$util; \
	done
	rm -rf $(DESTDIR)$(PREFIX)/share/xmpl/events
	rm -f $(DESTDIR)$(PREFIX)/share/xmpl/elegacy-session
	rm -f $(DESTDIR)$(PREFIX)/share/xsessions/elegacy.desktop

clean:
	rm -f $(OBJ) $(BIN) xmpl.o

manclean:
	cd man; $(MAKE) $(MFLAGS) clean
