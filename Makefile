PREFIX		= /usr/local
BINDIR		= $(PREFIX)/bin
MANDIR		= $(PREFIX)/share/man
BUILDDIR	= bin
SRC		= src
SRC_FILES	= $(wildcard $(SRC)/*.c)
TARGETS		= $(patsubst $(SRC)/%.c,%,$(SRC_FILES))

CC		= gcc
CCFLAGS		= -Wall -Wextra -Wpedantic
CCFLAGSPROG	=
CCLIBS		=
#CCFLAGSDEBUG	= -g
#CCLIBSSTATIC	= -static

all: $(TARGETS)

%: $(SRC)/%.c
	$(CC) $(CCFLAGS) $(CCFLAGSDEBUG) $(CCLIBSSTATIC) $(CCLIBS) $(CCFLAGSPROG) -DTARGET=\"$@\" -o $(BUILDDIR)/$@ $<

clean:
	rm -f $(addprefix $(BUILDDIR)/, $(TARGETS))

install: $(TARGETS)
	cp $(addprefix $(BUILDDIR)/, $(TARGETS)) $(BINDIR)

uninstall:
	rm -f $(addprefix $(BINDIR)/, $(TARGETS))
