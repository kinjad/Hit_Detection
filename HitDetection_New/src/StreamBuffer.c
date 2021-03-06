/* 
 * Implementing Linkedlist 
 * By Yuan Xie
 */
#include <stdio.h>
#include <stdlib.h>

#include "StreamBuffer.h"

/*Create a new list */
void InitBuffer(StreamBuffer* buffer) {
  buffer->head = 0;
  buffer->rear = 0;
  buffer->length = 0;
  for (int i = 0; i < FILTERSIZE; i++) {
    buffer->array[i] = 0;
  }
}

/*Add a node to a lsit 
 *@list the list you are working on
 *@content the element you want to add
*/
bool AddNode(StreamBuffer* buffer, float content) {
  
  if (buffer->length == FILTERSIZE) {
    printf("The List is full\n");
    return false;
  }
  //printf("adding %f\n", content);
  buffer->array[buffer->rear++] = content;
  buffer->length++;
  buffer->rear = buffer->rear % FILTERSIZE;
  return true;
}


/*Remove the first element
 *@list the list you are working on
*/
bool RemoveNode(StreamBuffer* buffer) {
  if (buffer->length == 0) {
    printf("The buffer is empty!\n");
    return false;
  }
  buffer->head++;
  buffer->head = buffer->head % FILTERSIZE;
  buffer->length--;
  return true;
}

/*Get the element of an given location
 *@list the list you are working on
 *@location the index of the node that you want you access
*/
float GetElement(StreamBuffer* buffer, int location) {
  if (location > buffer->length) {
    //printf("index overflow\n");
    return 0;
  }  
  return buffer->array[(buffer->head + location - 1) % FILTERSIZE];
}
//


/* Output all the elements in a list
 *@list the list you are working on
 */
void OutputAll(StreamBuffer* buffer) {
  printf("All the elements in a list: ");
  for (int i = 0; i < buffer->length; i++) {
    printf("%f, ", buffer->array[(buffer->head + i) % FILTERSIZE]);
  }
  printf("\n");
}

/* Calculate the dynamic sum of a buffer
 * @buffer the buffer you are working on
 * @old_sum the old sum of the buffer
 * @value the incoming new value
 */ 
float NewSum(StreamBuffer* buffer, float old_sum, float value) {
  
  //printf("In New_Sum %d, %f, %f, %f\n", buffer->head, buffer->array[buffer->head], value, old_sum);
  if (buffer->length == FILTERSIZE) {
    return old_sum - buffer->array[buffer->head] + value;        
  } else {
    return old_sum + value;
  }

}

/* Calculate the static sum of the buffer
 * @buffer the buffer you are working on
 */

float Sum(StreamBuffer* buffer) {
  float total = 0;
  int i;
  for (i = 0; i < buffer->length; i++) {
    total += buffer->array[(buffer->head + i) % FILTERSIZE];
  }
  return total;
}

