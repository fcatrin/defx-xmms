INCDIR= -I ../../include
SRCS = fx.c voice.c pan.c mod.c reverb.c
include ../../config.mak

LIBFX=../../lib/libfx.a

all: $(LIBFX)

clean :
	rm -f *.o $(LIBFX)

.c.o:$@
	$(CC) $(CFLAGS) $(INCDIR) $<

$(LIBFX): $(OBJS)
	$(AR) -r $(LIBFX) $(OBJS)

	