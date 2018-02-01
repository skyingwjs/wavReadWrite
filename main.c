#include <stdio.h>
#include <stdlib.h>
#include "wavread.h"
#include "wavwrite.h"


void main(){
	int i;
	char *file_name="es01_1.wav";
	char *new_wav_name="es01_2.wav";
	PcmFile *pf=NULL;
	WavFile *af=NULL;
	int samples_read=0;
	float wav_data[1024]={0};
	short wav_data1[1024]={0};
	int map[1]={0};

	pf=openWavRead(file_name, 0);//¶ÁÈ¡wavÎÄ¼þ 
	af=openWavWrite(new_wav_name,pf->samplerate,pf->channels,1,1,0);

	while(1){
		samples_read=wavReadFloat32(pf, wav_data, 1024,map);

		if (samples_read<1024){
			break;
		}
		for(i=0;i<1024;i++){
			wav_data1[i]=wav_data[i];
		}
		writeWavData(af,wav_data1,1024,0);
	}
	
	closeWavRead(pf);
	closeWavWrite(af);

	system("pause");
}

