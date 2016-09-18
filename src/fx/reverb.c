/* Reverberation module */
#include <math.h>
#include <fx/fx.h> 

/* Define's for circular buffer */
#define BufLen 4096
#define BufMask BufLen-1

#define nCombos 4  /* number comb filters */
#define nAllP 0  /* number of allpass filters */

#define Filters (nCombos + nAllP) /* total filters */

/* Standard module description */
FX fx_reverb = {
	FX_REVERB,
	0,
	3,
	fx_handle,
	fx_process,
	fx_init,
	fx_done
};

static int level=0,inlevel=0; /* Reverb and input level */
static double Len=0.0; /* reverb length in seconds */

/* Hard coded default delays in miliseconds */
static double DelayMsec[]={17,18,21,25,17,18,22,23};  

static short int DelaySamples[Filters*2]; /* delay length in sample units */
static short int BufPos[Filters*2]; /* delay actual position */ 
static long    Gain[Filters*2]; /* filter gain  */
static short int BigBuf[BufLen*Filters*2]; /* reverb Buffers data */
static short int *RevBuf[Filters*2]; /* reverb Buffers pointers*/

static double A, B,C; /* Filter Coefficients */

static void update(){  /* reverb gain precalc  */
	int i;
	for(i=0;i<Filters*2;i++) Gain[i]=(long)(exp((DelayMsec[i]/1000.0)*log(0.001)/Len)*0x10000);
}

/* Lowpass filter coefficients precalc */
static void CalcFilter(int freq){
	C=exp(-2*PI*(freq*0.8)/SAMPLERATE);
	B=-4*C/(1+C)*cos(2*PI*freq/SAMPLERATE);
	A=sqrt(1-B*B/(4*C))*(1-C);
}

static void fx_init(){  /* reverb initialization*/
	int i,j;

	for(i=0;i<Filters*2;i++) {
		RevBuf[i]=&BigBuf[i*BufLen]; /* point to actual data */
		BufPos[i]=0; /* reset delay position */
		for (j=0;j<BufLen;j++) RevBuf[i][j]=0; /* Clear delay buffer */
		/* precalc delay positions */
		DelaySamples[i]=(short int)(DelayMsec[i] *(double)SAMPLERATE / MSEC);
	}
	fx_handle(2,0); /* set default length */
}

/* Parameter handling, level, length (size), input level and filter */
static void fx_handle(int param, int value){
	switch (param){
		case 1: level=(int)value;break; /* 0..1 */
		case 2: Len=(value+1)/26.0; update(); break; /* 1/26..32/26 seconds */
		case 3: inlevel=(int)value;break; /* 0..1 */
		case 4: CalcFilter(value*300+2000);break; /* 2000..11600 Hz */
	}
}

static void fx_done(){
}

/* ufs.. here we go!  */
/* no allpass filter implementation */
static int fx_process(signed short *data, int len){
	long FBack,Comb,Out;
	short int In,i,j,k;
	static double y,yL1=0,yL2=0,yR1=0,yR2=0;
	short int **DelayBuf,*CurPos,*delay;
	long *gain;
	for(j=0;j<len;j++){  /* unrolled loop */
		In=data[j]>>2; /* input gain =0.5 */
		Comb=0; /* use pointer optimization */
		DelayBuf=RevBuf;
		gain=Gain;
		CurPos=BufPos;
		delay=DelaySamples;
		/* basically :
		    x(t) = current input
		    y(t) = current output
	    	    g = comb gain
		    y(t) = x(y) +  g*y(t-delay)
		 */ 
		for(i=0;i<Filters-nAllP;i++){ /* process comb filters */
			(*CurPos++)=((k=(*CurPos))+1) & BufMask;
			Comb+=(Out=(*DelayBuf)[(k-(*delay++)) & BufMask]);
			FBack= In + ((Out*(*gain++))>>0x10);
			(*DelayBuf++)[k]=sat(FBack);
		}
		/* apply low pass filter */
		y=(A*Comb - B*yL1) - C*yL2;
		yL2=yL1;
		yL1=y; 
		Comb=(int)y;
		/* easy way to move the input away */
		Out=((Comb*level)>>5) + ((data[j]*inlevel)>>5);

		data[j]=sat(Out);
		j++;

		In=data[j]>>2;  // primer canal
		Comb=0;
		for(i=0;i<Filters-nAllP;i++){
			(*CurPos++)=((k=(*CurPos))+1) & BufMask;
			Comb+=(Out=(*DelayBuf)[(k-(*delay++)) & BufMask]);
			FBack= In + ((Out*(*gain++))>>0x10);
			(*DelayBuf++)[k]=sat(FBack);
		}
		y=(A*Comb - B*yR1) - C*yR2;
		yR2=yR1;
		yR1=y; 
		Comb=(int)y;
		Out=((Comb*level)>>5) + ((data[j]*inlevel)>>5);

		data[j]=sat(Out);
	}
	return len;
}
