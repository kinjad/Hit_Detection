#include "StreamBuffer.h"

#define WAIT_LENGTH 100;

bool IsHit(float *sample);

float vel_cal(StreamBuffer *, float);

float mean_cal(StreamBuffer *);

float var_cal(StreamBuffer *);

float filterBuffer(StreamBuffer *, const float *);

void SetThreshHold(float *, float);

void SetRatio(float *, float);

void SetAcc(float *, float);

int CalLength(float *);












