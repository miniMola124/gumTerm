VTE_CFLAGS  := $(shell pkg-config --cflags vte-2.91)
VTE_LIBS    := $(shell pkg-config --libs vte-2.91)

GCC_FILES   := "main.c"
GCC_OUTFILE := "./gumterm"

all:
	gcc -O2 -Wall $(VTE_CFLAGS) $(GCC_FILES) -o $(GCC_OUTFILE) $(VTE_LIBS) -Wformat=2

run:
	$(GCC_OUTFILE)

makerun:
	gcc -O2 -Wall $(VTE_CFLAGS) $(GCC_FILES) -o $(GCC_OUTFILE) $(VTE_LIBS)
	$(GCC_OUTFILE)
