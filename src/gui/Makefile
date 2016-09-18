INCDIR= -I ../../include
SRCS = about.c gui.c mod.c pan.c reverb.c voice.c
include ../../config.mak

LIBFX=../../lib/libgui.a

all: $(LIBFX)

clean :
	rm -f *.o $(LIBFX)

.c.o:$@
	$(CC) $(CFLAGS) $(INCDIR) $<

$(LIBFX): $(OBJS)
	$(AR) -r $(LIBFX) $(OBJS)

	