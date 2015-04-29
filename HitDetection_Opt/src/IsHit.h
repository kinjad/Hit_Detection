#include "StreamBuffer.h"

#define WAITLENGTH 100

bool IsHit(float *sample);

//float vel_cal(StreamBuffer *, float);

//float mean_cal(StreamBuffer *);

//float var_cal(StreamBuffer *);

float FilterBuffer(StreamBuffer* , const float* );

void SetThreshHold(float* , float);

void SetRatio(float* , float);

void SetAcc(float* , float);

int CalLength(float* );

void ResizeBuffer(StreamBuffer* );

float PreFilterBuffer(StreamBuffer*, const float*, int index);












