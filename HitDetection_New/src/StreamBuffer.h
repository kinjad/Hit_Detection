#include <stdbool.h>

#define FILTERSIZE 100

//List structure
typedef struct StreamBuffer {
  int head;
  int rear;
  float array[FILTERSIZE];
  int length;
} StreamBuffer;

void InitBuffer(StreamBuffer* buffer);

bool AddNode(StreamBuffer* buffer, float content);

bool RemoveNode(StreamBuffer* buffer);

float GetElement(StreamBuffer* buffer, int location);

void OutputAll(StreamBuffer* buffer);

float NewSum(StreamBuffer* buffer, float old_sum, float value);

float Sum(StreamBuffer* buffer);
