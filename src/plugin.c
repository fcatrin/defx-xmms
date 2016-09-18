/* XMMS plugin interface */
#include <gtk/gtk.h>
#include <plugin.h>
#include <gui/about.h>
#include <gui/gui.h>
#include <fx/fx.h>

int debug=1;
char defxName[] = "DeFX for XMMS v0.9.8";

/* XMMS prototypes */
void query(AFormat *fmt, gint *rate, gint *nch);
int process(gpointer *data, gint length, AFormat fmt, gint srate, gint nch);
void init();
void done();
void config();

/* XMMS Effect Plugin definition */
EffectPlugin plugin = {
	NULL,
	NULL,
	defxName,
	init,
	done,
	about,
	config,
	process,
	query
};

/* tell XMMS the desired format... learn this winamp developers!! */
void query(AFormat *fmt, gint *rate, gint *nch) {
	*fmt=FMT_S16_LE;
	*rate=44100;
	*nch=2;
}

/* let XMMS call our global DeFX module */
int process(gpointer *data, gint length, AFormat fmt, gint srate, gint nch){
	if (data && length) defx_process(*data,length/2);
	return length;
}

/* just show our config window */
void config(){
	gui_show();
}

/* initialize our services */
void init(){
	defx_init();
	gui_init();
}

/* cleanup our services */
void done(){
	gui_done();
	defx_done();
}

/* Here we are XMMS! */
EffectPlugin *get_eplugin_info(){
	return &plugin;
}
