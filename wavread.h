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
typedef signed int int32_t;		//int32_t为有符号整型数4字节
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
  FILE *f;//文件结构
  int channels;//通道数
  int samplebytes;//样本字节数
  int samplerate;//采样率 由索引号来表示
  int samples;  //样本个数
  int bigendian; //大小端模式
  int isfloat;  //样本是否为浮点
} PcmFile;//自定义pcm文件结构

/******************************************************************************************
  *  @brief    打开一个用于读取数据的wav文件  
  *  @param[in]  path：wav文件路径； rawchans为0则读取wav文件头和波形数据，为1不读wav文件头，只读raw数据
  *  @param[out] 
  *  @Return:   PamFile * 指针，通过该指针可以获取channels，samplebytes，samples等信息
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
PcmFile *openWavRead(const char *path, int rawchans);//读取wav文件 


/******************************************************************************************
  *  @brief   读取指定数目的样本数据到bufffer中，读取的时域数据以float形式存储   
  *  @param[in]  buf：存储样本数据，num：需读取的样本点个数，map：通道映射（对于多声道需要将样本数据映射到
				 对应的声道上）
  *  @param[out] 
  *  @Return:  返回实际读取的样本点数  
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
size_t wavReadFloat32(PcmFile *sndf, float *buf, size_t num, int *map);//size_t 为int型 读取


/******************************************************************************************
  *  @brief     读取指定数目的样本数据到bufffer中，读取的时域数据以int(24位有效)形式存储    
  *  @param[in] buf：存储样本数据，num：需读取的样本点个数，map：通道映射（对于多声道需要将样本数据映射到
				对应的声道上） 
  *  @param[out] 
  *  @Return:   返回实际读取的样本点数   
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
size_t wavReadInt24(PcmFile *sndf, int32_t *buf, size_t num, int *map);



/******************************************************************************************
  *  @brief      关闭一个用于读取数据的wav文件
  *  @param[in]  PcmFile *file
  *  @param[out] 
  *  @Return:    返回0 表示成功关闭 
  *  @note       
  *  @date       2017.02.28
  *  @see        
  *****************************************************************************************/
int closeWavRead(PcmFile *file);//关闭wav文件

#ifdef __cplusplus
}
#endif
#endif 
