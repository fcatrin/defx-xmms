/* Reverb GTK Panel */
#include <gtk/gtk.h>
#include <gui/gui.h>
#include <fx/fx.h>

/* See mod.c comments */
#define nLevels 4
static char cfg_section[]="Reverb";
static const gchar *labels[]={"Level","Size","Near","Air"};
static int adj_def[]={12,9,6,2};
static GtkAdjustment *adj[nLevels];
static GtkToggleButton *enabled;

static void gui_reverb_init(){
	gui_cfg_panel_read(cfg_section,labels,adj,adj_def,nLevels,enabled);
}
		
static void gui_reverb_done(GtkWidget *w, gpointer data){
	gui_cfg_panel_write(cfg_section,labels,adj,nLevels,enabled);
}

void gui_reverb(GtkWidget *Container){
	GtkWidget *vbox;

	vbox = gtk_vbox_new(FALSE, 3);
	gtk_container_add(GTK_CONTAINER(Container),vbox);
	gtk_signal_connect(GTK_OBJECT(vbox),"destroy",GTK_SIGNAL_FUNC(gui_reverb_done),NULL);
		
	gui_label(vbox,"Reverberation");
	enabled=gui_toggle(vbox,FX_REVERB);
	gui_levels(vbox,nLevels,labels,FX_REVERB,adj);
	
	gui_reverb_init();
	gtk_widget_show(vbox);
}
