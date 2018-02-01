#ifdef _WIN32
#include <io.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

#include "wavwrite.h"


WavFile *openWavWrite(char *infile, int samplerate, int channels,
                            int outputFormat, int fileType, long channelMask)
{
    WavFile *wavFile = malloc(sizeof(WavFile));

    wavFile->outputFormat = outputFormat;

    wavFile->samplerate = samplerate;
    wavFile->channels = channels;
    wavFile->total_samples = 0;
    wavFile->fileType = fileType;
    wavFile->channelMask = channelMask;

    switch (outputFormat)
    {
    case WAV_FMT_16BIT:
        wavFile->bits_per_sample = 16;
        break;
    case WAV_FMT_24BIT:
        wavFile->bits_per_sample = 24;
        break;
    case WAV_FMT_32BIT:
    case WAV_FMT_FLOAT:
        wavFile->bits_per_sample = 32;
        break;
    default:
        if (wavFile) free(wavFile);
        return NULL;
    }

    if(infile[0] == '-')
    {
#ifdef _WIN32
        setmode(fileno(stdout), O_BINARY);
#endif
        wavFile->sndfile = stdout;
        wavFile->toStdio = 1;
    } else {
        wavFile->toStdio = 0;
        wavFile->sndfile = fopen(infile, "wb");
    }

    if (wavFile->sndfile == NULL)
    {
        if (wavFile) free(wavFile);
        return NULL;
    }

    if (wavFile->fileType == OUTPUT_WAV)
    {
        if (wavFile->channelMask)
            writeWavExtensibleHeader(wavFile, wavFile->channelMask);
        else
            writeWavHeader(wavFile);
    }

    return wavFile;
}

int writeWavData(WavFile *wavFile, void *sample_buffer, int samples, int offset)
{
    char *buf = (char *)sample_buffer;
    switch (wavFile->outputFormat)
    {
    case WAV_FMT_16BIT:
        return writeWav16bit(wavFile, buf + offset*2, samples);
    case WAV_FMT_24BIT:
        return writeWav24bit(wavFile, buf + offset*4, samples);
    case WAV_FMT_32BIT:
        return writeWav32bit(wavFile, buf + offset*4, samples);
    case WAV_FMT_FLOAT:
        return writeWavFloat(wavFile, buf + offset*4, samples);
    default:
        return 0;
    }

    return 0;
}

void closeWavWrite(WavFile *wavFile)
{
    if ((wavFile->fileType == OUTPUT_WAV) && (wavFile->toStdio == 0))
    {
        fseek(wavFile->sndfile, 0, SEEK_SET);

        if (wavFile->channelMask)
            writeWavExtensibleHeader(wavFile, wavFile->channelMask);
        else
            writeWavHeader(wavFile);
    }

    if (wavFile->toStdio == 0)
        fclose(wavFile->sndfile);

    if (wavFile) free(wavFile);
}

static int writeWavHeader(WavFile *wavFile)
{
    unsigned char header[44];
    unsigned char* p = header;
    unsigned int bytes = (wavFile->bits_per_sample + 7) / 8;
    float data_size = (float)bytes * wavFile->total_samples;
    unsigned long word32;

    *p++ = 'R'; *p++ = 'I'; *p++ = 'F'; *p++ = 'F';

    word32 = (data_size + (44 - 8) < (float)MAXWAVESIZE) ?
        (unsigned long)data_size + (44 - 8)  :  (unsigned long)MAXWAVESIZE;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    *p++ = 'W'; *p++ = 'A'; *p++ = 'V'; *p++ = 'E';

    *p++ = 'f'; *p++ = 'm'; *p++ = 't'; *p++ = ' ';

    *p++ = 0x10; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00;

    if (wavFile->outputFormat == WAV_FMT_FLOAT)
    {
        *p++ = 0x03; *p++ = 0x00;
    } else {
        *p++ = 0x01; *p++ = 0x00;
    }

    *p++ = (unsigned char)(wavFile->channels >> 0);
    *p++ = (unsigned char)(wavFile->channels >> 8);

    word32 = (unsigned long)(wavFile->samplerate + 0.5);
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    word32 = wavFile->samplerate * bytes * wavFile->channels;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    word32 = bytes * wavFile->channels;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);

    *p++ = (unsigned char)(wavFile->bits_per_sample >> 0);
    *p++ = (unsigned char)(wavFile->bits_per_sample >> 8);

    *p++ = 'd'; *p++ = 'a'; *p++ = 't'; *p++ = 'a';

    word32 = data_size < MAXWAVESIZE ?
        (unsigned long)data_size : (unsigned long)MAXWAVESIZE;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    return fwrite(header, sizeof(header), 1, wavFile->sndfile);
}

static int writeWavExtensibleHeader(WavFile *wavFile, long channelMask)
{
    unsigned char header[68];
    unsigned char* p = header;
    unsigned int bytes = (wavFile->bits_per_sample + 7) / 8;
    float data_size = (float)bytes * wavFile->total_samples;
    unsigned long word32;

    *p++ = 'R'; *p++ = 'I'; *p++ = 'F'; *p++ = 'F';

    word32 = (data_size + (68 - 8) < (float)MAXWAVESIZE) ?
        (unsigned long)data_size + (68 - 8)  :  (unsigned long)MAXWAVESIZE;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    *p++ = 'W'; *p++ = 'A'; *p++ = 'V'; *p++ = 'E';

    *p++ = 'f'; *p++ = 'm'; *p++ = 't'; *p++ = ' ';

    *p++ = /*0x10*/0x28; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00;

    /* WAVE_FORMAT_EXTENSIBLE */
    *p++ = 0xFE; *p++ = 0xFF;

    *p++ = (unsigned char)(wavFile->channels >> 0);
    *p++ = (unsigned char)(wavFile->channels >> 8);

    word32 = (unsigned long)(wavFile->samplerate + 0.5);
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    word32 = wavFile->samplerate * bytes * wavFile->channels;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    word32 = bytes * wavFile->channels;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);

    *p++ = (unsigned char)(wavFile->bits_per_sample >> 0);
    *p++ = (unsigned char)(wavFile->bits_per_sample >> 8);

    /* cbSize */
    *p++ = (unsigned char)(22);
    *p++ = (unsigned char)(0);

    /* WAVEFORMATEXTENSIBLE */

    /* wValidBitsPerSample */
    *p++ = (unsigned char)(wavFile->bits_per_sample >> 0);
    *p++ = (unsigned char)(wavFile->bits_per_sample >> 8);

    /* dwChannelMask */
    word32 = channelMask;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    /* SubFormat */
    if (wavFile->outputFormat == WAV_FMT_FLOAT)
    {
        /* KSDATAFORMAT_SUBTYPE_IEEE_FLOAT: 00000003-0000-0010-8000-00aa00389b71 */
        *p++ = 0x03;
        *p++ = 0x00;
        *p++ = 0x00;
        *p++ = 0x00;
        *p++ = 0x00; *p++ = 0x00; *p++ = 0x10; *p++ = 0x00; *p++ = 0x80; *p++ = 0x00;
        *p++ = 0x00; *p++ = 0xaa; *p++ = 0x00; *p++ = 0x38; *p++ = 0x9b; *p++ = 0x71;
    } else {
        /* KSDATAFORMAT_SUBTYPE_PCM: 00000001-0000-0010-8000-00aa00389b71 */
        *p++ = 0x01;
        *p++ = 0x00;
        *p++ = 0x00;
        *p++ = 0x00;
        *p++ = 0x00; *p++ = 0x00; *p++ = 0x10; *p++ = 0x00; *p++ = 0x80; *p++ = 0x00;
        *p++ = 0x00; *p++ = 0xaa; *p++ = 0x00; *p++ = 0x38; *p++ = 0x9b; *p++ = 0x71;
    }

    /* end WAVEFORMATEXTENSIBLE */



    *p++ = 'd'; *p++ = 'a'; *p++ = 't'; *p++ = 'a';

    word32 = data_size < MAXWAVESIZE ?
        (unsigned long)data_size : (unsigned long)MAXWAVESIZE;
    *p++ = (unsigned char)(word32 >>  0);
    *p++ = (unsigned char)(word32 >>  8);
    *p++ = (unsigned char)(word32 >> 16);
    *p++ = (unsigned char)(word32 >> 24);

    return fwrite(header, sizeof(header), 1, wavFile->sndfile);
}

static int writeWav16bit(WavFile *wavFile, void *sample_buffer,
                             unsigned int samples)
{
    int ret;
    unsigned int i;
    short *sample_buffer16 = (short*)sample_buffer;
    char *data = malloc(samples*wavFile->bits_per_sample*sizeof(char)/8);

    wavFile->total_samples += samples;

    if (wavFile->channels == 6 && wavFile->channelMask)
    {
        for (i = 0; i < samples; i += wavFile->channels)
        {
            short r1, r2, r3, r4, r5, r6;
            r1 = sample_buffer16[i];
            r2 = sample_buffer16[i+1];
            r3 = sample_buffer16[i+2];
            r4 = sample_buffer16[i+3];
            r5 = sample_buffer16[i+4];
            r6 = sample_buffer16[i+5];
            sample_buffer16[i] = r2;
            sample_buffer16[i+1] = r3;
            sample_buffer16[i+2] = r1;
            sample_buffer16[i+3] = r6;
            sample_buffer16[i+4] = r4;
            sample_buffer16[i+5] = r5;
        }
    }

    for (i = 0; i < samples; i++)
    {
        data[i*2] = (char)(sample_buffer16[i] & 0xFF);
        data[i*2+1] = (char)((sample_buffer16[i] >> 8) & 0xFF);
    }

    ret = fwrite(data, wavFile->bits_per_sample/8,samples,  wavFile->sndfile);

    if (data) free(data);

    return ret;
}

static int writeWav24bit(WavFile *wavFile, void *sample_buffer,
                             unsigned int samples)
{
    int ret;
    unsigned int i;
    long *sample_buffer24 = (long*)sample_buffer;
    char *data = malloc(samples*wavFile->bits_per_sample*sizeof(char)/8);

    wavFile->total_samples += samples;

    if (wavFile->channels == 6 && wavFile->channelMask)
    {
        for (i = 0; i < samples; i += wavFile->channels)
        {
            long r1, r2, r3, r4, r5, r6;
            r1 = sample_buffer24[i];
            r2 = sample_buffer24[i+1];
            r3 = sample_buffer24[i+2];
            r4 = sample_buffer24[i+3];
            r5 = sample_buffer24[i+4];
            r6 = sample_buffer24[i+5];
            sample_buffer24[i] = r2;
            sample_buffer24[i+1] = r3;
            sample_buffer24[i+2] = r1;
            sample_buffer24[i+3] = r6;
            sample_buffer24[i+4] = r4;
            sample_buffer24[i+5] = r5;
        }
    }

    for (i = 0; i < samples; i++)
    {
        data[i*3] = (char)(sample_buffer24[i] & 0xFF);
        data[i*3+1] = (char)((sample_buffer24[i] >> 8) & 0xFF);
        data[i*3+2] = (char)((sample_buffer24[i] >> 16) & 0xFF);
    }

    ret = fwrite(data, samples, wavFile->bits_per_sample/8, wavFile->sndfile);

    if (data) free(data);

    return ret;
}

static int writeWav32bit(WavFile *wavFile, void *sample_buffer,
                             unsigned int samples)
{
    int ret;
    unsigned int i;
    long *sample_buffer32 = (long*)sample_buffer;
    char *data = malloc(samples*wavFile->bits_per_sample*sizeof(char)/8);

    wavFile->total_samples += samples;

    if (wavFile->channels == 6 && wavFile->channelMask)
    {
        for (i = 0; i < samples; i += wavFile->channels)
        {
            long r1, r2, r3, r4, r5, r6;
            r1 = sample_buffer32[i];
            r2 = sample_buffer32[i+1];
            r3 = sample_buffer32[i+2];
            r4 = sample_buffer32[i+3];
            r5 = sample_buffer32[i+4];
            r6 = sample_buffer32[i+5];
            sample_buffer32[i] = r2;
            sample_buffer32[i+1] = r3;
            sample_buffer32[i+2] = r1;
            sample_buffer32[i+3] = r6;
            sample_buffer32[i+4] = r4;
            sample_buffer32[i+5] = r5;
        }
    }

    for (i = 0; i < samples; i++)
    {
        data[i*4] = (char)(sample_buffer32[i] & 0xFF);
        data[i*4+1] = (char)((sample_buffer32[i] >> 8) & 0xFF);
        data[i*4+2] = (char)((sample_buffer32[i] >> 16) & 0xFF);
        data[i*4+3] = (char)((sample_buffer32[i] >> 24) & 0xFF);
    }

    ret = fwrite(data, samples, wavFile->bits_per_sample/8, wavFile->sndfile);

    if (data) free(data);

    return ret;
}

static int writeWavFloat(WavFile *wavFile, void *sample_buffer,
                             unsigned int samples)
{
    int ret;
    unsigned int i;
    float *sample_buffer_f = (float*)sample_buffer;
    unsigned char *data = malloc(samples*wavFile->bits_per_sample*sizeof(char)/8);

    wavFile->total_samples += samples;

    if (wavFile->channels == 6 && wavFile->channelMask)
    {
        for (i = 0; i < samples; i += wavFile->channels)
        {
            float r1, r2, r3, r4, r5, r6;
            r1 = sample_buffer_f[i];
            r2 = sample_buffer_f[i+1];
            r3 = sample_buffer_f[i+2];
            r4 = sample_buffer_f[i+3];
            r5 = sample_buffer_f[i+4];
            r6 = sample_buffer_f[i+5];
            sample_buffer_f[i] = r2;
            sample_buffer_f[i+1] = r3;
            sample_buffer_f[i+2] = r1;
            sample_buffer_f[i+3] = r6;
            sample_buffer_f[i+4] = r4;
            sample_buffer_f[i+5] = r5;
        }
    }

    for (i = 0; i < samples; i++)
    {
        int exponent, mantissa, negative = 0 ;
        float in = sample_buffer_f[i];

        data[i*4] = 0; data[i*4+1] = 0; data[i*4+2] = 0; data[i*4+3] = 0;
        if (in == 0.0)
            continue;

        if (in < 0.0)
        {
            in *= -1.0;
            negative = 1;
        }
        in = (float)frexp(in, &exponent);
        exponent += 126;
        in *= (float)0x1000000;
        mantissa = (((int)in) & 0x7FFFFF);

        if (negative)
            data[i*4+3] |= 0x80;

        if (exponent & 0x01)
            data[i*4+2] |= 0x80;

        data[i*4] = mantissa & 0xFF;
        data[i*4+1] = (mantissa >> 8) & 0xFF;
        data[i*4+2] |= (mantissa >> 16) & 0x7F;
        data[i*4+3] |= (exponent >> 1) & 0x7F;
    }

    ret = fwrite(data, samples, wavFile->bits_per_sample/8, wavFile->sndfile);

    if (data) free(data);

    return ret;
}
