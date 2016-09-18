/* Configuration save/restore routines */
#include <stdlib.h>
#include <xmms/configfile.h>
#include "config.h"

/* use standard xmms config file routines */
ConfigFile *cfg_file=NULL;
char cfg_def_filename[]="/.xmms/config.DeFX";
char cfg_filename[100]="";
static int cfg_mode=0;
gchar *cfg_section;

/* load the file into an internal structure */
int cfg_init(int mode, gchar *section){
	if (!strlen(cfg_filename)) {
		strcpy(cfg_filename,getenv("HOME"));
		strcat(cfg_filename,cfg_def_filename);
	}
	if (cfg_file) return 0;
	else if (!(cfg_file=xmms_cfg_open_file(cfg_filename))) cfg_file=xmms_cfg_new();
	cfg_mode=mode;
	cfg_section=section;
	return 1;
}

/* flush and write configuration */
void cfg_done(){
	if (cfg_file) {
		if (cfg_mode==CFG_WRITE) xmms_cfg_write_file(cfg_file,cfg_filename);
		xmms_cfg_free(cfg_file);
		cfg_file=NULL;
	}
}

/* application write config value routine */
void cfg_write(const gchar *key, gint value){
	if (!cfg_file) return;
	xmms_cfg_write_int(cfg_file,cfg_section,(gchar *)key,value);
}

/* the same for read */
gint cfg_read(const gchar *key, gint def){
	int  value;
	if (!cfg_file) return 0;
	if (!xmms_cfg_read_int(cfg_file,cfg_section,(gchar *)key,&value)) value=def;
	return value;
}

