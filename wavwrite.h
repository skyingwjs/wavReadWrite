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
  *  @brief  创建一个用于写样本数据的wav文件    
  *  @param[in]  infile：文件名，samplerate：采样率，channnels：声道数，outputFormat：输出格式（4种）
				 分别为 WAV_FMT_16BIT，WAV_FMT_24BIT，WAV_FMT_32BIT ，WAV_FMT_FLOAT
				 fileType:输出文件类型（2种）OUTPUT_WAV ， OUTPUT_RAW
				 channelMask 通道装换（为0表示不需要进行声道装换）
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
  *  @brief      将指定数目的样本数据写入创建的wav文件中
  *  @param[in]  sample_buffer：样本数据，可以是short，int或者float，但必须与openWavWrite形参中的
				 outputFormat一致。如short（WAV_FMT_16BIT）,int(WAV_FMT_16BIT或WAV_FMT_32BIT)，
				 float（WAV_FMT_FLOAT）;samples:需要写入的样本数量； offset： 偏移（默认设0）
  *  @param[out] 
  *  @Return:    返回实际写入wav文件的样本数
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
int writeWavData(WavFile *wavFile, 
				 void *sample_buffer, 
				 int samples, 
				 int offset);


/******************************************************************************************
  *  @brief     关闭一个用于写数据的wav文件 
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
