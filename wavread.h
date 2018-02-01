#ifndef _WAV_READ_H_
#define _WAV_READ_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifndef __MPEG4IP_INCLUDED__
/* Let's avoid some boring conflicting declarations */
#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif

#ifndef HAVE_INT32_T
typedef signed int int32_t;		//int32_tΪ�з���������4�ֽ�
#endif
#ifndef HAVE_INT16_T
typedef signed short int16_t;
#endif
#ifndef HAVE_U_INT32_T
typedef unsigned int u_int32_t;
#endif
#ifndef HAVE_U_INT16_T
typedef unsigned short u_int16_t;
#endif
#endif 

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
  FILE *f;//�ļ��ṹ
  int channels;//ͨ����
  int samplebytes;//�����ֽ���
  int samplerate;//������ ������������ʾ
  int samples;  //��������
  int bigendian; //��С��ģʽ
  int isfloat;  //�����Ƿ�Ϊ����
} PcmFile;//�Զ���pcm�ļ��ṹ

/******************************************************************************************
  *  @brief    ��һ�����ڶ�ȡ���ݵ�wav�ļ�  
  *  @param[in]  path��wav�ļ�·���� rawchansΪ0���ȡwav�ļ�ͷ�Ͳ������ݣ�Ϊ1����wav�ļ�ͷ��ֻ��raw����
  *  @param[out] 
  *  @Return:   PamFile * ָ�룬ͨ����ָ����Ի�ȡchannels��samplebytes��samples����Ϣ
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
PcmFile *openWavRead(const char *path, int rawchans);//��ȡwav�ļ� 


/******************************************************************************************
  *  @brief   ��ȡָ����Ŀ���������ݵ�bufffer�У���ȡ��ʱ��������float��ʽ�洢   
  *  @param[in]  buf���洢�������ݣ�num�����ȡ�������������map��ͨ��ӳ�䣨���ڶ�������Ҫ����������ӳ�䵽
				 ��Ӧ�������ϣ�
  *  @param[out] 
  *  @Return:  ����ʵ�ʶ�ȡ����������  
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
size_t wavReadFloat32(PcmFile *sndf, float *buf, size_t num, int *map);//size_t Ϊint�� ��ȡ


/******************************************************************************************
  *  @brief     ��ȡָ����Ŀ���������ݵ�bufffer�У���ȡ��ʱ��������int(24λ��Ч)��ʽ�洢    
  *  @param[in] buf���洢�������ݣ�num�����ȡ�������������map��ͨ��ӳ�䣨���ڶ�������Ҫ����������ӳ�䵽
				��Ӧ�������ϣ� 
  *  @param[out] 
  *  @Return:   ����ʵ�ʶ�ȡ����������   
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
size_t wavReadInt24(PcmFile *sndf, int32_t *buf, size_t num, int *map);



/******************************************************************************************
  *  @brief      �ر�һ�����ڶ�ȡ���ݵ�wav�ļ�
  *  @param[in]  PcmFile *file
  *  @param[out] 
  *  @Return:    ����0 ��ʾ�ɹ��ر� 
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
int closeWavRead(PcmFile *file);//�ر�wav�ļ�

#ifdef __cplusplus
}
#endif
#endif 
