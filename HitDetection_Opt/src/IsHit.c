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
  float ACC_THRESHOLD = 2000;
  static int buffer_size = 0;
  //Indicating whether the system is in low power mode
  static bool high_flag = false;
  //Indicating whether the system has initialized the two intermediate buffers
  static bool done_flag = false;
  //The high power time counter
  static int detect_count = 0; 
  
  

  
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
    InitBuffer(&mSampleBufferY, LARGEFILTERSIZE);
    //InitBuffer(&mSampleBufferZ, mFilter_length, 0);
    //InitBuffer(&mPsLowpassBufferX, mFilter_length, 0);
    InitBuffer(&mPsLowpassBufferY, FILTERSIZE);
    //InitBuffer(&mPsLowpassBufferZ, mFilter_length, 0);
    //InitBuffer(&mPsSampleBufferX, mFilter_length, 0);
    InitBuffer(&mPsSampleBufferY, FILTERSIZE);
    //InitBuffer(&mPsSampleBufferZ, mFilter_length, 0);    
    //InitBuffer(&AcclBuffer, 1200, 0);
  }


  // If the length of acceleration vector exceeds the threshold, enter the high power mode
  //printf("the sample is %f\n", sampleY);

  if (sampleXYZ >= ACC_THRESHOLD && !high_flag) {
    high_flag = true;
  }

  // If the vector flag is not set, simply push the raw sample into the buffer and return


  if (mSampleBufferY.length == LARGEFILTERSIZE) {
    RemoveNode(&mSampleBufferY, LARGEFILTERSIZE);    
  }     
  AddNode(&mSampleBufferY, sampleY, LARGEFILTERSIZE);

  if (!high_flag) {
    buffer_size++;
    return false;
  }



  //Else as has entered the high power mode, first push the raw sample into the buffer
  /*
  if (mSampleBufferY.length == FILTERSIZE) {
    RemoveNode(&mSampleBufferY, FILTERSIZE);    
  }     
  AddNode(&mSampleBufferY, sampleY, FILTERSIZE);

  if (buffer_size == 804) {
    printf("Small buffer:\n");
    OutputAll(&mSampleBufferY, LARGEFILTERSIZE);   
  }
  */

  // If the high power flag is set for the first time. Intialize the two intermediate buffers



  if (!done_flag && high_flag) {
    for (int i = 0; i < FILTERSIZE; i++) {
      //Calculate two samples
      lpSample = PreFilterBuffer(&mSampleBufferY, mFilter, i);
      rawSample = GetElement(&mSampleBufferY, mMidLocation + i, LARGEFILTERSIZE);             
      //Update related sums

      /*

      mPsSample += rawSample * rawSample;
      mPsLowpass += lpSample * lpSample;

      */
      
      //printf("%f\n", rawSample);

      mPsSample = NewSum(&mPsSampleBufferY, mPsSample, rawSample * rawSample);
      mPsLowpass = NewSum(&mPsLowpassBufferY, mPsLowpass, lpSample * lpSample);


      //Push the intermediate samples into their buffers
      if (mPsLowpassBufferY.length == FILTERSIZE) {
        RemoveNode(&mPsLowpassBufferY, FILTERSIZE);    
      }     
      AddNode(&mPsLowpassBufferY, lpSample * lpSample, FILTERSIZE);
      
      if (mPsSampleBufferY.length == FILTERSIZE) {
        RemoveNode(&mPsSampleBufferY, FILTERSIZE);    
      }     
      AddNode(&mPsSampleBufferY, rawSample * rawSample, FILTERSIZE);

    }
    //printf("%f\n", mPsSample);
    //ResizeBuffer(&mSampleBufferY);
    /*
    printf("Raw buffer after resizing\n");
    OutputAll(&mSampleBufferY, FILTERSIZE);      
    */
    //Reset the detect_count
    detect_count = 0;
    done_flag = true;
  }

  
  lpSample = FilterBuffer(&mSampleBufferY, mFilter);
  rawSample = GetElement(&mSampleBufferY, mMidLocation + FILTERSIZE, LARGEFILTERSIZE);

  //printf("%f\n", lpSample);

  mPsSample = NewSum(&mPsSampleBufferY, mPsSample, rawSample * rawSample);
  mPsLowpass = NewSum(&mPsLowpassBufferY, mPsLowpass, lpSample * lpSample);

  if (mPsLowpassBufferY.length == FILTERSIZE) {
    RemoveNode(&mPsLowpassBufferY, FILTERSIZE);    
  }     
  AddNode(&mPsLowpassBufferY, lpSample * lpSample, FILTERSIZE);

  if (mPsSampleBufferY.length == FILTERSIZE) {
    RemoveNode(&mPsSampleBufferY, FILTERSIZE);    
  }     
  AddNode(&mPsSampleBufferY, rawSample * rawSample, FILTERSIZE);


  ratioY = mPsLowpass / mPsSample;
  psHighpassY = mPsSample - mPsLowpass;


  // Test whether the filter is full or not
  buffer_size++;
  
  if (buffer_size <FILTERSIZE) {
    printf("The filter is not full yet\n");
    return false;
  }
  
  //printf("%f\n", mPsSample);

  if ((ratioY < RATIO) && (psHighpassY > THRESHOLD) && sampleXYZ > A_ABS_THRESHOLD) {

    return true;
  } else {
    detect_count++;

    if ((detect_count == WAITLENGTH) && high_flag) {
      high_flag = false;
      detect_count = 0;
      done_flag = false;
    }

    return false;
  }
}



/* Filter the buffer
 * @buffer the buffer you are working on
 * @the filter you use
 */

float FilterBuffer(StreamBuffer *buffer, const float *filter) {
  float total = 0;
  for (int i = 0; i < FILTERSIZE; i++) {
    //total += filter[i] * buffer->array[(buffer->head + FILTERSIZE - 1 - i) % FILTERSIZE];
    //printf("%f\n", buffer->array[(buffer->head + FILTERSIZE + i) % LARGEFILTERSIZE]);
    total += filter[FILTERSIZE - 1 - i] * buffer->array[(buffer->head + FILTERSIZE + i) % LARGEFILTERSIZE];
  }   
  return total;
}


/* Initially Filter the buffer
 * @buffer the buffer you are working on
 * @filter the filter you use
 * @index the index of the element that you are trying to recover for the intermediate buffer
 */

float PreFilterBuffer(StreamBuffer *buffer, const float *filter, int index) {
  float total = 0;
  int length = buffer->length;
  for (int i = 0; i < FILTERSIZE; i++) {
    //total += filter[i] * buffer->array[(buffer->head + length - 1 - i) % FILTERSIZE];
    //total += filter[i] * buffer->array[(buffer->head + index + FILTERSIZE - 1 - i) % LARGEFILTERSIZE];
    total += filter[FILTERSIZE - 1 - i] * buffer->array[(buffer->head + index + i) % LARGEFILTERSIZE];
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



/* Resize the buffer, leaving only 100 newest samples
 * @buffer the buffer that you are working on
 *
 */
void ResizeBuffer(StreamBuffer* buffer) {
  float temp[FILTERSIZE];
  int temp_size = 0;
  int limit = (FILTERSIZE > buffer->length)? buffer->length : FILTERSIZE;
  int index = 0;
  if (limit < FILTERSIZE) {
    index = 0;
  } else {
    index = buffer->length - FILTERSIZE;
  }
  for (int i = 0; i < limit ; i++) {
    temp[i] = buffer->array[(buffer->head + i + index) % LARGEFILTERSIZE];
    temp_size++;
  }

  for (int i = 0; i < temp_size; i++) {
    buffer->array[i] = temp[i];    
  }
  buffer->length = temp_size;
}
