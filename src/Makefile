include ../config.mak
INCDIR= -I ../include
SRCS = config.c plugin.c

LIB = ../lib/libmain.a	
$(LIB): $(OBJS)
	$(AR) -r $(LIB) $(OBJS)
 

clean :
	rm -f *.o $(LIB)

.c.o:$@
	$(CC) $(CFLAGS) $(INCDIR) $<

	