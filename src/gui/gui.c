/* Main GUI window and common routines */
#include <gtk/gtk.h>
#include <gui/gui.h>
#include <gui/mod.h>
#include <gui/voice.h>
#include <gui/pitch.h>
#include <gui/pan.h>
#include <gui/reverb.h>
#include <config.h>
#include <plugin.h>

#define nPanels 4

GtkWidget *main_win=NULL;

typedef void (* gui_panel) (GtkWidget *box);

/* Read panel configuration */
void gui_cfg_panel_read(char *cfg_section,const char *labels[], 
	GtkAdjustment *adj[],int adj_def[],int nLevels, 
	GtkToggleButton *enabled){
		
	int i;
	cfg_init(CFG_READ,cfg_section);
	for (i=0;i<nLevels;i++)
		gtk_adjustment_set_value(adj[i],cfg_read(labels[i],adj_def[i]));
	gtk_toggle_button_set_active(enabled,cfg_read("Enabled",0));
	cfg_done();
}

/* Write panel configuration */
void gui_cfg_panel_write(char *cfg_section,const char *labels[],
		GtkAdjustment *adj[],int nLevels, GtkToggleButton *enabled){
			
	int i;
	cfg_init(CFG_WRITE,cfg_section);
	cfg_write("Enabled",enabled->active);
	for(i=0;i<nLevels;i++)
		cfg_write(labels[i],(int)adj[i]->value); 
	cfg_done();
}

/* some toggle button was pressed */
void gui_fx_toggle(GtkWidget *widget,gpointer value){
	defx_param((int)value>>16,0,GTK_TOGGLE_BUTTON (widget)->active);
}

/* Create toggle button */
GtkToggleButton *gui_toggle(GtkWidget *parent, int id){
	GtkWidget *but;	
	
	but=gtk_toggle_button_new_with_label("ON");
	gtk_signal_connect(GTK_OBJECT(but),"toggled",
		GTK_SIGNAL_FUNC(gui_fx_toggle),(gpointer)(id<<16));
	gtk_box_pack_start(GTK_BOX(parent),but,FALSE,TRUE,0);
	gtk_widget_show(but);
	return GTK_TOGGLE_BUTTON(but);
}	

/* Create Label */
void gui_label(GtkWidget *box,const gchar *lbl){
	GtkWidget *title;
	
	title = gtk_label_new(lbl);
	gtk_box_pack_start(GTK_BOX(box),title,FALSE,FALSE,0);
	gtk_widget_show(title);
}

/* Some parameter (Adjustment) has changed */
void gui_fx_param(GtkAdjustment *adj, gpointer value){
	defx_param((int)value>>16,(int)value & 15,(gint)adj->value);
}

/* Create Adjustment controls */
void gui_levels(GtkWidget *box,int nLevels,const gchar *labels[],
		int id, GtkAdjustment *Adj[]){
			
	GtkWidget *vbox, *hbox, *level, *label;
	GtkObject *adj;
	int i;
	
	hbox=gtk_hbox_new(TRUE,nLevels);
	gtk_box_pack_start(GTK_BOX(box),hbox,FALSE,FALSE,0);
	
	for(i=0;i<nLevels;i++){
		vbox=gtk_vbox_new(FALSE,2);
		gtk_box_pack_start(GTK_BOX(hbox),vbox,FALSE,FALSE,0);
		
		adj=gtk_adjustment_new(32.0,0.0,32.0,1.0,4.0,0.0);
		if (Adj) Adj[i]=GTK_ADJUSTMENT(adj);
		level=gtk_vscale_new(GTK_ADJUSTMENT(adj));
		gtk_signal_connect(GTK_OBJECT(adj),"value_changed",
			GTK_SIGNAL_FUNC(gui_fx_param),(gpointer )((id<<16)+i+1));
		gtk_scale_set_draw_value (GTK_SCALE (level), FALSE);  
		label=gtk_label_new(labels[i]);
		gtk_box_pack_start(GTK_BOX(vbox),level,FALSE,FALSE,0);
		gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,0);
		
		gtk_widget_show(label);
		gtk_widget_show(level);
		gtk_widget_show(vbox);
	}
	gtk_widget_show(hbox);
}

/* Create main GUI window */
void gui_init(){
	int i;
	GtkWidget *hbox, *panel;
	gui_panel gui_panels[nPanels]={gui_voice,gui_pan,gui_mod,gui_reverb};

	if (main_win)return;
	main_win = gtk_window_new(GTK_WINDOW_DIALOG);
	gtk_signal_connect(GTK_OBJECT(main_win), "destroy",
		GTK_SIGNAL_FUNC(gtk_widget_destroyed), &main_win);
	gtk_window_set_title(GTK_WINDOW(main_win), defxName);
	gtk_window_set_policy(GTK_WINDOW(main_win), FALSE, FALSE, FALSE);
	gtk_window_set_position(GTK_WINDOW(main_win), GTK_WIN_POS_MOUSE);

	hbox = gtk_hbox_new(FALSE, nPanels);
	gtk_container_add(GTK_CONTAINER(main_win), hbox);

	for (i=0;i<nPanels;i++){
		panel=gtk_handle_box_new();
		gtk_container_set_border_width(GTK_CONTAINER(panel),5);
		if (gui_panels[i]) gui_panels[i](panel);
		
		gtk_box_pack_start(GTK_BOX(hbox),panel, FALSE, FALSE,0);
		gtk_widget_show(panel);
	}	

	gtk_widget_show(hbox);
	
	cfg_init(CFG_READ,"GTK_GUI");
	if (cfg_read("Enabled",1)) gui_show();
	cfg_done();
}

int gui_shown=0;

/* Show our GUI Window */
void gui_show(){	
	if (!main_win) gui_init();
	gtk_widget_show(main_win);
	gui_shown=1;
}

/* Destroy our GUI Window */
void gui_done(){
	cfg_init(CFG_WRITE,"GTK_GUI");
	cfg_write("Enabled",(main_win&&gui_shown)?1:0);
	cfg_done();

	if (main_win) gtk_widget_destroy(main_win);
}
