/* Chorus, Flange and Phaser module */

#include <math.h>
#include <fx/fx.h> 

/* Circular buffer Length and binary mask */
#define BufLen 4096
#define BufMask BufLen-1

/* Valid LFO waveforms */
#define wavSine 0
#define wavTriangle 1
#define wavExp 2

/* Macro to compute current delay buffer position */
#define GetPos(a) (int)(BufLen*((double)(a)/LFORate))

/* Macro to compute current delay LFO waveform position */
#define CalcPos(a) (int)((a)*(double)LFORate/BufLen)

/* Internal defaults */ 
static double Depth=5.0;	/* depth[msec] */
static double Delay=20.0;	/* delay[msec] */
static double Rate=0.1;		/* rate[Hz]    */

static int level=0;			/* level           */
static int FeedBack=0;		/* feedback[0..1]  */
static int Phase=0;			/* stereo de-phase */

/* Initial buffer positions */
static int LFORate=8192;	/* LFO Rate (fixed point) */
static int LFOPos=0;		/* LFO Position           */
static int BufPos=0;		/* LFO Buffer Position    */


/* Circular buffers: Left, Right and LFO waveform */
static signed short BufL[BufLen];	/* Left Delay Buffer */
static signed short BufR[BufLen];	/* Right Delay Buffer */
static signed short LFO[BufLen];	/* Precalculated LFO waveform */

static void fx_update_phaser();		/* update phaser state */
static int fx_mod_phaser(signed short *data, int len); /* phaser processing */
static int fx_mod_flanger(signed short *data, int len); /* flange & chorus */


double pha, phaBottom, phaTop;		/* phaser filter status and range */
double phaStep, phaStepRate;		/* phaser change helper vars */

/* Structure to describe this effect module, see fx/fx.h */
FX fx_mod = {
	FX_MOD,
	0,
	6,
	fx_handle,
	fx_mod_flanger,
	fx_init,
	fx_done
};

/* Precalculate the LFO waveform */
static void WaveForm(int wave){
	int i,j=BufLen/2;		/* buffer pointers */
	short int depth,delay;  /* profundidad y delay del flange/chorus */
	static int type=wavSine;/* type of waveform, Sine by default */
	
	if (wave>=0) type=wave; /* get LFO waveform type */
	
	/* Transform miliseconds to sample units */
	depth = (short int)(Depth * SAMPLERATE / MSEC);
 	delay = (short int)(Delay * SAMPLERATE / MSEC);
 	
	switch (type){
		case wavSine:
			for(i=0;i<BufLen;i++) 
				LFO[i]=2+delay+depth/2-(short int)((double)(depth/2)*sin(PI2*(double)i/(double)BufLen));
			break;
		case wavTriangle:
			for(i=0;i<j;i++) {
				LFO[i]=2+delay+(short int)(depth*((double)i/(double)j));
				LFO[i+j]=2+delay+(short int)(depth*(1.0-(double)i/(double)j));
			}
			break;
		case wavExp:
			for (i=0;i<BufLen;i++)
				LFO[i]=2+delay+depth-(short int)(depth*((double)pow((double)(i-j)/(double)j,2.0))); 
	}
}

/* Change waveform and calculate the nearest delay position
   to avoid clicking  */
static void update(int wave){  
	int i,old=LFO[GetPos(LFOPos)],min=4096,minpos,aux;
	WaveForm(wave);
	for (i=0;i<BufLen;i++) 
		if ((aux=abs(old-LFO[i]))<min) {
			min=aux;
			minpos=i;
		}
	LFOPos=CalcPos(minpos);
}

/* Module initialization */
static void fx_init(){
	int i;
	for (i=0;i<BufLen;i++) {
		BufL[i]=0;
		BufR[i]=0;
	}
}

/* Module cleanup */
static void fx_done(){
}

/* Handle parameters: level, LFO, depth, Rate, Feedback, 
   Stereo separation, Phaser & Chorus/flange selection */
static void fx_handle(int param, int value){
	switch(param){
		
		case 1 : /* level [0..1] */
			level=value;break;
			
		case 2 : /* LFO waverform type */
			value=(value<10?wavSine:(value<20?wavTriangle:wavExp)); 
			update(value);break;
			
		case 3 : /* LFO Depth 0 ~ 16.0 [ms] */
			Depth=value/2.0; 
			fx_update_phaser();
			update(-1);break; 
			
		case 4 : /* LFO Rate 0.03 ~ 0.3  [Hz] */
			Rate=((value+1)/96.0);
			LFORate = (long)(SAMPLERATE/Rate);
			fx_update_phaser();
			break; 
			
		case 5 : /* Feedback -1 ~ 1 */
			FeedBack=value-16;break; 
			
		case 6 : Phase=(int)((value/64.0)*BufLen);
				 break;
		case 7 : switch (32-value){
					case 0 : 
					case 1 : Delay=(32-value)?0.0:20.0;
							update(-1);
							fx_mod.process=fx_mod_flanger;
							break;
					case 2 : fx_mod.process=fx_mod_phaser;
							break;
				}	
	}
}


static void fx_update_phaser(){
    double range = pow(2.0,Depth);
    
    phaBottom = (PI * 100.0) / SAMPLERATE;
    phaTop = phaBottom * range;  // log scale
    phaStepRate = pow(range,Rate / (SAMPLERATE / 2));
    
    pha = phaBottom;
    phaStep = phaStepRate;
    
}


/* Phaser is based on an anonymous source code I've found on Internet */
/* Please, if you are the author, contact with us */
static int fx_mod_phaser(signed short *data, int len){
    double smpin, smpout,l,r,c;
    static double lx1,ly1,lx2,ly2,lx3,ly3,lx4,ly4;
    static double rx1,ry1,rx2,ry2,rx3,ry3,rx4,ry4;
    int i;
    
    for (i=0;i<len;i++){
        c = (1.0 - pha) / (1.0 + pha);	/* calc coef for current freq */
        
        smpin=data[i];

        l = smpin + (FeedBack*ly4)/17;	/* apply feedback */
        ly1 = c * (ly1 + l) - lx1;	/* go with filtering */
        lx1 = l;
        ly2 = c * (ly2 + ly1) - lx2;
        lx2 = ly1;
        ly3 = c * (ly3 + ly2) - lx3;
        lx3 = ly2;
        ly4 = c * (ly4 + ly3) - lx4;
        lx4 = ly3;

        /* prepare output */
        smpout = ((int)(ly4 * level + smpin *(0x40-level)))>>6;         
        data[i]=sat(smpout);
        i++;
        
		/* Process next channel */
        smpin=data[i];
        
        r = smpin + (FeedBack*ry4)/17; 
        ry1 = c * (ry1 + r) - rx1;
        rx1 = r;
        ry2 = c * (ry2 + ry1) - rx2;
        rx2 = ry1;
        ry3 = c * (ry3 + ry2) - rx3;
        rx3 = ry2;
        ry4 = c * (ry4 + ry3) - rx4;
        rx4 = ry3;

        smpout = ((int)(ry4 * level + smpin *(0x40-level)))>>6;         
        data[i]=sat(smpout);
        
        /* update filter */
        pha *= phaStep;
        if(pha > phaTop) phaStep = 1.0 / phaStepRate;
        else if(pha < phaBottom) phaStep = phaStepRate;
    }
	return len;
}


/* Flange/Chorus effect */
static int fx_mod_flanger(signed short *data, int len){
	long smpout,smpin,Delay; /* sample out/in, delay position */
	int j,t; /* input buffer position, base delay position */
	for(j=0;j<len;j++){

		/* First channel procesing */
		Delay=LFO[t=GetPos(LFOPos)]; /* Delay Position */
		if (++LFOPos>=LFORate ) LFOPos=0; /* Update LFO position */
		
		Delay = (BufPos - Delay) & BufMask; /* Apply circular mask */
		smpin  = data[j];smpout = BufL[Delay]; /* fetch data */

		/* Mix input with buffered input */
		smpout = (smpout * level + smpin*(0x40-level))>>6; 
		smpin += ((smpout * FeedBack)>>4); /* apply feedback */
		
		/* Write output value and store in delay buffer */
		BufL[BufPos] = sat(smpin); data[j]=sat(smpout); 

		j++;

		/* Process next channel */
		Delay = LFO[(t+Phase) & BufMask];
		Delay = (BufPos - Delay) & BufMask;

		smpin  = data[j];smpout = BufR[Delay];
		smpout = (smpout * level + smpin*(0x40-level))>>6;
		smpin += ((smpout * FeedBack)>>4) ;
		BufR[BufPos] = sat(smpin); data[j]=sat(smpout); 
		
		data[j]=sat(smpout);
		BufPos = (++BufPos) & BufMask;

	}
	return len;
}
