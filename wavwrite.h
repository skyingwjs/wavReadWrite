#ifndef _WAV_WRITE_H_
#define _WAV_WRITE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#define MAXWAVESIZE     4294967040LU

#define OUTPUT_WAV 1
#define OUTPUT_RAW 2

#define WAV_FMT_16BIT  1
#define WAV_FMT_24BIT  2
#define WAV_FMT_32BIT  3
#define WAV_FMT_FLOAT  4
#define WAV_FMT_DOUBLE 5

typedef struct
{
    int toStdio;
    int outputFormat;
    FILE *sndfile;
    unsigned int fileType;
    unsigned long samplerate;
    unsigned int bits_per_sample;
    unsigned int channels;
    unsigned long total_samples;
    long channelMask;
} WavFile;

/******************************************************************************************
  *  @brief  ����һ������д�������ݵ�wav�ļ�    
  *  @param[in]  infile���ļ�����samplerate�������ʣ�channnels����������outputFormat�������ʽ��4�֣�
				 �ֱ�Ϊ WAV_FMT_16BIT��WAV_FMT_24BIT��WAV_FMT_32BIT ��WAV_FMT_FLOAT
				 fileType:����ļ����ͣ�2�֣�OUTPUT_WAV �� OUTPUT_RAW
				 channelMask ͨ��װ����Ϊ0��ʾ����Ҫ��������װ����
  *  @param[out] 
  *  @Return:    WavFile *
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
WavFile *openWavWrite(char *infile, 
					  int samplerate, 
					  int channels,
                      int outputFormat, 
				      int fileType, 
					  long channelMask);


/******************************************************************************************
  *  @brief      ��ָ����Ŀ����������д�봴����wav�ļ���
  *  @param[in]  sample_buffer���������ݣ�������short��int����float����������openWavWrite�β��е�
				 outputFormatһ�¡���short��WAV_FMT_16BIT��,int(WAV_FMT_16BIT��WAV_FMT_32BIT)��
				 float��WAV_FMT_FLOAT��;samples:��Ҫд������������� offset�� ƫ�ƣ�Ĭ����0��
  *  @param[out] 
  *  @Return:    ����ʵ��д��wav�ļ���������
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
int writeWavData(WavFile *wavFile, 
				 void *sample_buffer, 
				 int samples, 
				 int offset);


/******************************************************************************************
  *  @brief     �ر�һ������д���ݵ�wav�ļ� 
  *  @param[in]  
  *  @param[out] 
  *  @Return:    
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
void closeWavWrite(WavFile *wavFile);



static int writeWavHeader(WavFile *wavFile);

static int writeWavExtensibleHeader(WavFile *wavFile, 
									long channelMask);

static int writeWav16bit(WavFile *wavFile, 
						 void *sample_buffer,
                         unsigned int samples);

static int writeWav24bit(WavFile *wavFile, 
						 void *sample_buffer,
                         unsigned int samples);

static int writeWav32bit(WavFile *wavFile, 
						 void *sample_buffer,
                         unsigned int samples);

static int writeWavFloat(WavFile *wavFile, 
						 void *sample_buffer,
                         unsigned int samples);


#ifdef __cplusplus
}
#endif
#endif
