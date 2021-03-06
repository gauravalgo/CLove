#ifndef MIXER_DEFS_H
#define MIXER_DEFS_H

#include "AL/alc.h"
#include "AL/al.h"
#include "alMain.h"
#include "alu.h"

struct MixGains;
struct MixGainMono;

struct HrtfParams;
struct HrtfState;

/* C resamplers */
const ALfloat *Resample_copy32_C(const ALfloat *src, ALuint frac, ALuint increment, ALfloat *restrict dst, ALuint dstlen);
const ALfloat *Resample_point32_C(const ALfloat *src, ALuint frac, ALuint increment, ALfloat *restrict dst, ALuint dstlen);
const ALfloat *Resample_lerp32_C(const ALfloat *src, ALuint frac, ALuint increment, ALfloat *restrict dst, ALuint dstlen);
const ALfloat *Resample_cubic32_C(const ALfloat *src, ALuint frac, ALuint increment, ALfloat *restrict dst, ALuint dstlen);


/* C mixers */
void MixDirect_Hrtf_C(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                      ALuint Counter, ALuint Offset, ALuint OutPos, const ALuint IrSize,
                      const struct HrtfParams *hrtfparams, struct HrtfState *hrtfstate,
                      ALuint BufferSize);
void MixDirect_C(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                 struct MixGains *Gains, ALuint Counter, ALuint OutPos,
                 ALuint BufferSize);
void MixSend_C(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
               struct MixGainMono *Gain, ALuint Counter, ALuint OutPos,
               ALuint BufferSize);

/* SSE mixers */
void MixDirect_Hrtf_SSE(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                        ALuint Counter, ALuint Offset, ALuint OutPos, const ALuint IrSize,
                        const struct HrtfParams *hrtfparams, struct HrtfState *hrtfstate,
                        ALuint BufferSize);
void MixDirect_SSE(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                   struct MixGains *Gains, ALuint Counter, ALuint OutPos,
                   ALuint BufferSize);
void MixSend_SSE(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                 struct MixGainMono *Gain, ALuint Counter, ALuint OutPos,
                 ALuint BufferSize);

/* SSE resamplers */
inline void InitiatePositionArrays(ALuint frac, ALuint increment, ALuint *frac_arr, ALuint *pos_arr, ALuint size)
{
    ALuint i;

    pos_arr[0] = 0;
    frac_arr[0] = frac;
    for(i = 1;i < size;i++)
    {
        ALuint frac_tmp = frac_arr[i-1] + increment;
        pos_arr[i] = pos_arr[i-1] + (frac_tmp>>FRACTIONBITS);
        frac_arr[i] = frac_tmp&FRACTIONMASK;
    }
}

const ALfloat *Resample_lerp32_SSE2(const ALfloat *src, ALuint frac, ALuint increment,
                                    ALfloat *restrict dst, ALuint numsamples);
const ALfloat *Resample_lerp32_SSE41(const ALfloat *src, ALuint frac, ALuint increment,
                                     ALfloat *restrict dst, ALuint numsamples);

/* Neon mixers */
void MixDirect_Hrtf_Neon(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                         ALuint Counter, ALuint Offset, ALuint OutPos, const ALuint IrSize,
                         const struct HrtfParams *hrtfparams, struct HrtfState *hrtfstate,
                         ALuint BufferSize);
void MixDirect_Neon(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                    struct MixGains *Gains, ALuint Counter, ALuint OutPos,
                    ALuint BufferSize);
void MixSend_Neon(ALfloat (*restrict OutBuffer)[BUFFERSIZE], const ALfloat *data,
                  struct MixGainMono *Gain, ALuint Counter, ALuint OutPos,
                  ALuint BufferSize);

#endif /* MIXER_DEFS_H */
