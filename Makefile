include config.mak

COMMONLIBS = lib/libfx.a lib/libgui.a lib/libmain.a
DEFX = lib/libdefx.so
LIBDIR = -Llib

all: $(DEFX)

$(DEFX):  $(COMMONLIBS)
	$(CC) $(LIBDIR) -shared src/plugin.o  -o $@ $(COMMONLIBS) 
	cp $(DEFX) ..

clean :
	$(MAKE) -C src/fx clean
	$(MAKE) -C src/gui clean
	$(MAKE) -C src/ clean
	rm $(DEFX)

lib/libfx.a:
	$(MAKE) -C src/fx
	
lib/libgui.a:
	$(MAKE) -C src/gui

lib/libmain.a:
	$(MAKE) -C src
	
install: $(DEFX)
	install -o root -g root -m 755 -s -D $(DEFX) $(DESTDIR)`xmms-config --effect-plugin-dir`/libdefx.so
