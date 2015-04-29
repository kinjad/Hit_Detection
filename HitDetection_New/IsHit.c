#include <stdio.h>
#include <math.h>
#include <time.h>
#include "IsHit.h"

bool IsHit(float *sample) {
  

  //Precalcuated Filter
  const static float COEF_LOWPASS_FILTER_FS400_FC40[101] = {-7.46743430298208e-05f,0.00248354664696717f,0.00112233989588931f,0.000826711931638179f,0.000220573657162887f,-0.000608580698353681f,-0.00142362923704699f,-0.00193774629305945f,-0.00190779145478616f,-0.00122833673267619f,2.42038943507627e-06f,0.00147768089448461f,0.00274754750201222f,0.00334708032125518f,0.00295224327906748f,0.00151424369613786f,-0.000675392159140040f,-0.00302409269580415f,-0.00478596570411362f,-0.00528393145988946f,-0.00414492280150041f,-0.00147314448490976f,0.00211013271842175f,0.00558635587549593f,0.00781088756279274f,0.00786402930626426f,0.00538476051055825f,0.000769312457516857f,-0.00482890710808253f,-0.00977060467662383f,-0.0123560386272336f,-0.0113712281545797f,-0.00654299033953943f,0.00122950409704551f,0.00996487932906945f,0.0170356688095536f,0.0198720879212355f,0.0167565749717718f,0.00749089935821721f,-0.00627112584227792f,-0.0211593051091526f,-0.0327111848494703f,-0.0363731364604173f,-0.0286490449499551f,-0.00811165965449541f,0.0239952523706242f,0.0637512209042429f,0.105192151754557f,0.141456823496643f,0.166209945155485f,0.174995192947581f,0.166209945155485f,0.141456823496643f,0.105192151754557f,0.0637512209042429f,0.0239952523706242f,-0.00811165965449541f,-0.0286490449499551f,-0.0363731364604173f,-0.0327111848494703f,-0.0211593051091526f,-0.00627112584227792f,0.00749089935821721f,0.0167565749717718f,0.0198720879212355f,0.0170356688095536f,0.00996487932906945f,0.00122950409704551f,-0.00654299033953943f,-0.0113712281545797f,-0.0123560386272336f,-0.00977060467662383f,-0.00482890710808253f,0.000769312457516857f,0.00538476051055825f,0.00786402930626426f,0.00781088756279274f,0.00558635587549593f,0.00211013271842175f,-0.00147314448490976f,-0.00414492280150041f,-0.00528393145988946f,-0.00478596570411362f,-0.00302409269580415f,-0.000675392159140040f,0.00151424369613786f,0.00295224327906748f,0.00334708032125518f,0.00274754750201222f,0.00147768089448461f,2.42038943507627e-06f,-0.00122833673267619f,-0.00190779145478616f,-0.00193774629305945f,-0.00142362923704699f,-0.000608580698353681f,0.000220573657162887f,0.000826711931638179f,0.00112233989588931f,0.00248354664696717f,-7.46743430298208e-05f};

  //Paramters
  float THRESHOLD = 5000;
  float RATIO = 0.9;
  float A_ABS_THRESHOLD = 12.0;
  const float *mFilter = COEF_LOWPASS_FILTER_FS400_FC40;
  int mFilter_length = FILTERSIZE;
  int mMidLocation = (mFilter_length - 1) / 2;
  static float mPsSample = 0;
  static float mPsLowpass = 0;
  float ratio = 1;
  float prevRatio = 1;
  float psHighpass = 0;
  static int buffer_size = 0;
  static bool flag;
  
  

  
  //Buffers
  //static StreamBufferFloat mSampleBufferX; 
  static StreamBuffer mSampleBufferY;
  //static StreamBufferFloat mSampleBufferZ;
  //static StreamBufferFloat mPsLowpassBufferX;
  static StreamBuffer mPsLowpassBufferY;
  //static StreamBufferFloat mPsLowpassBufferZ;
  //static StreamBufferFloat mPsSampleBufferX;
  static StreamBuffer mPsSampleBufferY;
  //static StreamBufferFloat mPsSampleBufferZ;
  

  // yh addition start  
  //static StreamBufferFloat AcclBuffer;
  // yh addition start
  
  float sampleX = sample[0];
  float sampleY = sample[1];
  float sampleZ = sample[2];
  float sampleXYZ = (float) sqrt(sampleX * sampleX + sampleY * sampleY + sampleZ * sampleZ);
  float lpSample;
  float rawSample;
  float ratioY;
  float psHighpassY;



  //Initialize buffers
  if (buffer_size == 0) {
    //InitBuffer(&mSampleBufferX, mFilter_length, 0);
    InitBuffer(&mSampleBufferY);
    //InitBuffer(&mSampleBufferZ, mFilter_length, 0);
    //InitBuffer(&mPsLowpassBufferX, mFilter_length, 0);
    InitBuffer(&mPsLowpassBufferY);
    //InitBuffer(&mPsLowpassBufferZ, mFilter_length, 0);
    //InitBuffer(&mPsSampleBufferX, mFilter_length, 0);
    InitBuffer(&mPsSampleBufferY);
    //InitBuffer(&mPsSampleBufferZ, mFilter_length, 0);    
    //InitBuffer(&AcclBuffer, 1200, 0);
  }




  // deal with X 

  /*

  Push(&mSampleBufferX, sampleX);
  lpSample = filterBuffer(&mSampleBufferX, mFilter);  
  rawSample = Get(&mSampleBufferX, mMidLocation);
  Push(&mPsLowpassBufferX, lpSample * lpSample);
  Push(&mPsSampleBufferX, rawSample * rawSample);
  mPsSample = Sum(&mPsSampleBufferX);
  mPsLowpass = Sum(&mPsLowpassBufferX);
  float ratioX = mPsLowpass / mPsSample;
  float psHighpassX = mPsSample - mPsLowpass;

  */



  // deal with Y

  //start = clock();

  

  if (mSampleBufferY.length == FILTERSIZE) {
    RemoveNode(&mSampleBufferY);    
  }     
  AddNode(&mSampleBufferY, sampleY);
  
  lpSample = filterBuffer(&mSampleBufferY, mFilter);
  rawSample = GetElement(&mSampleBufferY, mMidLocation);

  //printf("%f\n", rawSample);

  mPsSample = NewSum(&mPsSampleBufferY, mPsSample, rawSample * rawSample);
  mPsLowpass = NewSum(&mPsLowpassBufferY, mPsLowpass, lpSample * lpSample);




  if (mPsLowpassBufferY.length == FILTERSIZE) {
    RemoveNode(&mPsLowpassBufferY);    
  }     
  AddNode(&mPsLowpassBufferY, lpSample * lpSample);

  if (mPsSampleBufferY.length == FILTERSIZE) {
    RemoveNode(&mPsSampleBufferY);    
  }     
  AddNode(&mPsSampleBufferY, rawSample * rawSample);


  ratioY = mPsLowpass / mPsSample;
  psHighpassY = mPsSample - mPsLowpass;



  


  /*
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("part time : %f\n", cpu_time_used);  
  */

  //deal with Z

  /*

  Push(&mSampleBufferZ, sampleZ);
  lpSample = filterBuffer(&mSampleBufferZ, mFilter);
  rawSample = Get(&mSampleBufferZ, mMidLocation);
  Push(&mPsLowpassBufferZ, lpSample * lpSample);
  Push(&mPsSampleBufferZ, rawSample * rawSample);
  mPsSample = Sum(&mPsSampleBufferZ);
  mPsLowpass = Sum(&mPsLowpassBufferZ);
  float ratioZ = mPsLowpass / mPsSample;
  float psHighpassZ = mPsSample - mPsLowpass;

  */




  // Test whether the filter is full or not
  buffer_size++;
  if (buffer_size <FILTERSIZE) {
    printf("The filter is not full yet\n");
    return false;
  }


  //printf("%f, %f, %f\n", ratioY, psHighpassY, sampleXYZ);
  

  

  if ((ratioY < RATIO) && (psHighpassY > THRESHOLD) && sampleXYZ > A_ABS_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

/*
 *Issure remains about size
 */
/*
float vel_cal(StreamBuffer *AcclBuffer, float sampleA) {
  if (AcclBuffer->length > 100) {
    RemoveNode(AcclBuffer);
  }
  AddElement(AcclBuffer, sampleA);
  float vel_sensor = Sum(AcclBuffer) / 400;
  return vel_sensor;
}
*/

/* Calculate the mean of the buffer
 * @buffer the buffer you are working on
 */
/*
float mean_cal(StreamBuffer *buffer) {
  float total = 0;
  total = Sum(buffer);
  return total / buffer->length;
}
*/
/* Calculate variance of the buffer?
 * @buffer the buffer you are working on
 */
/*
float var_cal(StreamBuffer *buffer) {
  float total = 0;
  for (int i = 0; i < buffer->length; i++) {
    total += pow(buffer->array[i] - mean_cal(buffer), 2);
  }
  return total / buffer->length;
}
*/
/* Filter the buffer
 * @buffer the buffer you are working on
 * @the filter you use
 */

float filterBuffer(StreamBuffer *buffer, const float *filter) {
  float total = 0;
  if (buffer->length == FILTERSIZE) {
    int length = buffer->length;
    for (int i = 0; i < length; i++) {
      total += filter[i] * buffer->array[(buffer->head + length - 1 - i) % FILTERSIZE];

    }    
  }
  return total;
}

/* Set the Threshold
 * @THRESHOLD the threshold 
 * @t the value that you want to set to
 */
void SetThreshHold(float* THRESHOLD, float t) {
  *THRESHOLD = t;
}

/* Set the ratio
 * @RATIO the threshold 
 * @r the value that you want to set to
 */
void SetRatio(float* RATIO, float r) {
  *RATIO = r;
}

/* Set the acc
 * @A_ABS_THRESHOLD 
 * @acc the value you want to set to
 */

void SetAcc(float *A_ABS_THRESHOLD, float acc) {
  *A_ABS_THRESHOLD = (acc >= 9.81)? acc : 9.81;
}



