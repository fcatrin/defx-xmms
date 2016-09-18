/* Main FX processing module */

#include <fx/fx.h>
#include <fx/voice.h>
#include <fx/pan.h>
#include <fx/mod.h>
#include <fx/reverb.h>


/* instances of FX modules */
FX *defx[]={&fx_voice,&fx_pan,&fx_mod,&fx_reverb,0}; 

/* Global FX initializations: Just disable each FX,
   call the initializer of each one and shut down each parameter*/
void defx_init(){
	int i,j;
	for(i=0;defx[i];i++) {
		defx[i]->enabled=0;
		if (defx[i]->init) defx[i]->init();
		for (j=0;j<defx[i]->nParams;j++) if(defx[i]->handle) defx[i]->handle(j,0);
	}
}

/* Global destructor : call the destructor of each module */
void defx_done(){
	int i;
	for(i=0;defx[i];i++) if (defx[i]->done) defx[i]->done();
}

/* Global parameter handling: using the FX ID, pass to the 
   particular handler.  Parameter 0 is always effect on/off  */
void defx_param(int id, int param, int value){
	int i;
	for(i=0;defx[i];i++) if (defx[i]->handle && defx[i]->id==id) {
		if (param) defx[i]->handle(param,32-value);
		else defx[i]->enabled=value;
	}
}

/* Global handling routine: delegate to modules */	
void defx_process(signed short *data, int len){
	int i;
	for(i=0;defx[i];i++) if (defx[i]->process && defx[i]->enabled) defx[i]->process(data,len);
}
