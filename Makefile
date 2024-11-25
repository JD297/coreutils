PREFIX		= /usr/local
BINDIR		= $(PREFIX)/bin
MANDIR		= $(PREFIX)/share/man
LIBDIR		= $(PREFIX)/lib
INCLUDEDIR	= $(PREFIX)/include

BUILDDIR	= bin
SRC		= src
SRC_FILES	= $(wildcard $(SRC)/*.c)
TARGETS		= $(patsubst $(SRC)/%.c,%,$(SRC_FILES))

CC		= gcc
CCFLAGS		= -Wall -Wextra -Wpedantic -nodefaultlibs -W -ffreestanding
CCFLAGSPROG	=
CCLIBS		= -l:libc.a
CCLIB		= -L $(LIBDIR)
CCINCLUDE	= -I $(INCLUDEDIR)
#CCFLAGSDEBUG	= -g
CCLIBSSTATIC	= -static

all: $(TARGETS)

%: $(SRC)/%.c
	$(CC) $(CCINCLUDE) $(CCFLAGS) $(CCFLAGSDEBUG) $(CCLIBSSTATIC) $(CCFLAGSPROG) -DTARGET=\"$@\" -o $(BUILDDIR)/$@ $< $(CCLIB) $(CCLIBS)

clean:
	rm -f $(addprefix $(BUILDDIR)/, $(TARGETS))

install: $(TARGETS)
	cp $(addprefix $(BUILDDIR)/, $(TARGETS)) $(BINDIR)

uninstall:
	rm -f $(addprefix $(BINDIR)/, $(TARGETS))
