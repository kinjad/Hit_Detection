#include <stdbool.h>

#define FILTERSIZE 100
#define LARGEFILTERSIZE 200

//List structure
typedef struct StreamBuffer {
  int head;
  int rear;
  float array[LARGEFILTERSIZE];
  int length;
} StreamBuffer;

void InitBuffer(StreamBuffer* , int);

bool AddNode(StreamBuffer* , float , int);

bool RemoveNode(StreamBuffer* , int);

float GetElement(StreamBuffer* , int, int);

void OutputAll(StreamBuffer* , int);

float NewSum(StreamBuffer* , float , float );

float Sum(StreamBuffer* , int );
