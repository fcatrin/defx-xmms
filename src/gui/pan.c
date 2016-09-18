/* Panning GTK Panel */
#include <gtk/gtk.h>
#include <gui/gui.h>
#include <fx/fx.h>
#include <config.h>

#define nLevels 1
/* see mod.c comments */
static char cfg_section[]="Balance";
static const gchar *labels[nLevels]={"Level"};
static int adj_def[]={32};
static GtkAdjustment *adj[nLevels];
static GtkToggleButton *enabled;

static void gui_pan_init(){
	gui_cfg_panel_read(cfg_section,labels,adj,adj_def,nLevels,enabled);
}
		
static void gui_pan_done(GtkWidget *w, gpointer data){
	gui_cfg_panel_write(cfg_section,labels,adj,nLevels,enabled);
}

void gui_pan(GtkWidget *Container){
	GtkWidget *vbox;
	const gchar *labels[]={"Pos"};

	vbox = gtk_vbox_new(FALSE, 3);
	gtk_container_add(GTK_CONTAINER(Container),vbox);
	gtk_signal_connect(GTK_OBJECT(vbox),"destroy",GTK_SIGNAL_FUNC(gui_pan_done),NULL);
		
	gui_label(vbox,"Pan");
	enabled=gui_toggle(vbox,FX_PAN);
	gui_levels(vbox,nLevels,labels,FX_PAN,adj);
	
	gui_pan_init();
	gtk_widget_show(vbox);
	
}
