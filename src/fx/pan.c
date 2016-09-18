/* Panning module */
#include <fx/fx.h>

/* Panning position */
static int level=32;

/* Module description */
FX fx_pan = {
	FX_PAN,
	0,
	1,
	fx_handle,
	fx_process,
	fx_init,
	fx_done
};

/* Easy parameter handling */
static void fx_handle(int param, int value){
	if (value>=0 && value<=32) level=value;
}

/* Move left or right channel to front witouth debalancing */
static int fx_process(signed short *data, int len){
	int i,l,r;
	for (i=0;i<len;i+=2) {
		l=data[i];
		r=data[i+1];
		l=level>=16?((32-level)*l+(level-16)*r)>>4:l;
		r=level<16?((16-level)*l+level*r)>>4:r;
		data[i]=l;
		data[i+1]=r;
	}
	return len;
}

static void fx_init(){
}

static void fx_done(){
}
