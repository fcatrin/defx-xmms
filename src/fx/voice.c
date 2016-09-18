/* voice removal module */
#include <math.h>
#include <fx/fx.h>

/* effect level */
static int level=32;

/* standard module definition */
FX fx_voice = {
	FX_VOICE,
	0,
	4,
	fx_handle,
	fx_process,
	fx_init,
	fx_done
};

/* center band and bandwith of filter */
static double Band=440.0,Width=100.0;
/* filter coefficients */
static double A,B,C;
/* filtered mono signal level */
static int MonoLevel;

/* filter coefficients precalc */
static void update(){
	C=exp(-2*PI*Width/SAMPLERATE);
	B=-4*C/(1+C)*cos(2*PI*Band/SAMPLERATE);
	A=sqrt(1-B*B/(4*C))*(1-C);
}


/* Parameter handling : level, filtered mono level, center freq, bandwidth */
static void fx_handle(int param, int value){
	switch (param) {
		case 1:level=value;break;
		case 2:MonoLevel=value;break;
		case 3:Band=value*10;update();break;
		case 4:Width=value*4;update();break;
	}
	
}

/* Ok, get a great laugh.. you didn't think it was so easy? */
/* The filter was a Claudio Nicora (Italy) suggestion */
static int fx_process(signed short *data, int len){
	int i,r,l,o,x;
	static double y,y1=0,y2=0;
	for (i=0;i<len;i+=2) {
		/* get left and right inputs */
		l=data[i];
		r=data[i+1];
		/* Do filtering */
		x=(l+r)>>1;
		y=(A*x - B*y1) - C*y2;
		y2=y1;
		y1=y; 
		/* filter mono signal */
		o=(int)(y*(MonoLevel/10.0)); 
		o=sat(o);
		o=(o*level)>>5;
		/* now cut the center! */
		x=l-((r*level)>>5) + o;
		r=r-((l*level)>>5) + o ;
		data[i]= sat(x);
		data[i+1]=sat(r); 
		
	}
	return len;
}

static void fx_init(){
}

static void fx_done(){
}
