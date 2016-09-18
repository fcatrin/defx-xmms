/* Header file for standard GUI routines */

/* Create lables */
extern void gui_label(GtkWidget *box,const gchar *lbl);

/* Create level bars for continuos parameters */
extern void gui_levels(GtkWidget *box,int nLevels,const gchar *labels[], int id, GtkAdjustment *adj[]);

/* Create toggle buttons */
extern GtkToggleButton *gui_toggle(GtkWidget *parent, int id);

/* Read module settings */
void gui_cfg_panel_read(char *cfg_section,const gchar *labels[], GtkAdjustment *adj[],int adj_def[],int nLevels, GtkToggleButton *enabled);

/* Write module settings */
void gui_cfg_panel_write(char *cfg_section,const char *labels[], GtkAdjustment *adj[],int nLevels, GtkToggleButton *enabled);

/* Main GUI initialization */
extern void gui_init();

/* Show GUI */
extern void gui_show();

/* Destroy GUI */
void gui_done();

