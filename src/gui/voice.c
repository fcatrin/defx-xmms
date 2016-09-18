/* Voice removal GTK interface */
#include <gtk/gtk.h>
#include <gui/gui.h>
#include <fx/fx.h>
#include <config.h>

#define nLevels 4
static char cfg_section[]="Karaoke";
static const gchar *labels[nLevels]={"Level","Filter","Band","Width"};
static int adj_def[]={0,6,22,9};
static GtkAdjustment *adj[nLevels];
static GtkToggleButton *enabled;

static void gui_voice_init(){
	gui_cfg_panel_read(cfg_section,labels,adj,adj_def,nLevels,enabled);
}
		
static void gui_voice_done(GtkWidget *w, gpointer data){
	gui_cfg_panel_write(cfg_section,labels,adj,nLevels,enabled);
}

void gui_voice(GtkWidget *Container){
	GtkWidget *vbox;

	vbox = gtk_vbox_new(FALSE, 3);
	gtk_container_add(GTK_CONTAINER(Container),vbox);
	gtk_signal_connect(GTK_OBJECT(vbox),"destroy",GTK_SIGNAL_FUNC(gui_voice_done),NULL);
		
	gui_label(vbox,"Karaoke");
	enabled=gui_toggle(vbox,FX_VOICE);
	gui_levels(vbox,nLevels,labels,FX_VOICE,adj);
	
	gui_voice_init();
	gtk_widget_show(vbox);
	
}
