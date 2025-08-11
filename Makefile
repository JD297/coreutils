.POSIX:

CC            = cc
CFLAGS        = -Wall -Wextra -Wpedantic -g
LDFLAGS       =

PREFIX        = /usr/local
BINDIR        = $(PREFIX)/bin
MANDIR        = $(PREFIX)/share/man
SRCDIR        = src
BUILDDIR      = build

all: $(BUILDDIR)/cat $(BUILDDIR)/env $(BUILDDIR)/ls $(BUILDDIR)/halt $(BUILDDIR)/uname

$(BUILDDIR)/cat: $(SRCDIR)/cat.c
	$(CC) $(CFLAGS) $(SRCDIR)/cat.c -o $@ $(LDFLAGS)

$(BUILDDIR)/env: $(SRCDIR)/env.c
	$(CC) $(CFLAGS) $(SRCDIR)/env.c -o $@ $(LDFLAGS)

$(BUILDDIR)/ls: $(SRCDIR)/ls.c
	$(CC) $(CFLAGS) $(SRCDIR)/ls.c -o $@ $(LDFLAGS)

$(BUILDDIR)/halt: $(SRCDIR)/halt.c
	$(CC) $(CFLAGS) $(SRCDIR)/halt.c -o $@ $(LDFLAGS)

$(BUILDDIR)/uname: $(SRCDIR)/uname.c
	$(CC) $(CFLAGS) $(SRCDIR)/uname.c -o $@ $(LDFLAGS)

clean:
	rm $(BUILDDIR)/*

install: all
	cp $(BUILDDIR)/cat $(BINDIR)
	cp $(BUILDDIR)/env $(BINDIR)
	cp $(BUILDDIR)/ls $(BINDIR)
	cp $(BUILDDIR)/halt $(BINDIR)
	cp $(BUILDDIR)/uname $(BINDIR)

uninstall:
	rm -f $(BINDIR)/cat
	rm -f $(BINDIR)/env
	rm -f $(BINDIR)/ls
	rm -f $(BINDIR)/halt
	rm -f $(BINDIR)/uname
