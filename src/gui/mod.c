/* Phaser, Chorus/Flange GTK panel */
#include <gtk/gtk.h>
#include <gui/gui.h>
#include <fx/fx.h>
#include <config.h>

/* Number of adj controls */
#define nLevels 6

/* Type of FX's */
#define MOD_CHORUS 0
#define MOD_FLANGER 1
#define MOD_PHASER 2
#define MOD_TOTAL 3

/* Config section */
static char cfg_section[]="MOD";
/* Adj names */
static const gchar *labels[nLevels]={"Level","LFO","Depth","Rate","FBack","Stereo"};
/* Adj default values */
static int adj_def[nLevels]={6,16,26,25,8,0};
/* Adj controls */
static GtkAdjustment *adj[nLevels];
/* Button controls */
static GtkToggleButton *enabled,*types[MOD_TOTAL];
/* Type of effects */
static int type=MOD_CHORUS;

/* panel constructor */
static void gui_mod_init(){
	gui_cfg_panel_read(cfg_section,labels,adj,adj_def,nLevels,enabled);
	cfg_init(CFG_READ,cfg_section);
	gtk_toggle_button_set_active(types[cfg_read("Type",MOD_FLANGER)],1);
	cfg_done();
}

/* panel destructor */		
static void gui_mod_done(GtkWidget *w, gpointer data){
	gui_cfg_panel_write(cfg_section,labels,adj,nLevels,enabled);
	cfg_init(CFG_WRITE,cfg_section);
	cfg_write("Type",type);
	cfg_done();
}

/* change FX type */
static gui_mod_type(GtkRadioButton *w, gpointer data){
	if (GTK_TOGGLE_BUTTON(w)->active) {
		type=(int)data;
		defx_param(FX_MOD,nLevels+1,type);
	}
}

/* Add Radio Button control */	
GtkWidget *gui_mod_addradio(GtkWidget *box, GSList *group, const gchar *label, int id){
	GtkWidget *but;
	but=gtk_radio_button_new_with_label(group,label);
	types[id]=GTK_TOGGLE_BUTTON(but);
	gtk_signal_connect(GTK_OBJECT(but),"toggled",GTK_SIGNAL_FUNC(gui_mod_type),(gpointer)id);
	gtk_box_pack_start(GTK_BOX(box),but,FALSE,FALSE,0);
	gtk_widget_show(but);
	return but;
}	

/* Panel Top Section */	
void gui_mod_top(GtkWidget *parent){
	GtkWidget *but,*box;
	GSList *group=NULL;
	
	box=gtk_hbox_new(FALSE,4);
	gtk_box_pack_start(GTK_BOX(parent),box,FALSE,FALSE,0);
	
	but=gui_mod_addradio(box,NULL,"Chorus",MOD_CHORUS);
	group=gtk_radio_button_group(GTK_RADIO_BUTTON(but)); 
	gui_mod_addradio(box,group,"Flanger",MOD_FLANGER);
	group=gtk_radio_button_group(GTK_RADIO_BUTTON(but)); 
	gui_mod_addradio(box,group,"Phaser",MOD_PHASER);
	enabled=gui_toggle(box,FX_MOD);
	
	gtk_widget_show(box);
}	

/* Panel main section */
void gui_mod(GtkWidget *Container){
	GtkWidget *vbox;

	vbox = gtk_vbox_new(FALSE, 3);
	gtk_container_add(GTK_CONTAINER(Container),vbox);
	gtk_signal_connect(GTK_OBJECT(vbox),"destroy",GTK_SIGNAL_FUNC(gui_mod_done),NULL);
		
	gui_label(vbox,"Frequency Modulation");
	gui_mod_top(vbox);
	gui_levels(vbox,nLevels,labels,FX_MOD,adj);
	
	gui_mod_init();
	gtk_widget_show(vbox);
	
}
