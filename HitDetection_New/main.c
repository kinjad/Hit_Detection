#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IsHit.h"
void GenerateSamples(float samples[][3]);
void ReadData(float samples[][3]);
int main() {
  float samples[1200][3];
  ReadData(samples);
  //GenerateSamples(samples);

  clock_t start, end;
  double cpu_time_used;

  start = clock();    

  for (int i = 0; i < 1200; i++) {
    printf("Testing the %dth sample: \n", i + 1);

    if (IsHit(samples[i])) {
      printf("Hit\n");
    } else {
      printf("Not hit\n");
    }
  }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("total time : %f\n", cpu_time_used);  


  return 1;
}

void GenerateSamples(float samples[][3]) {
  srand(time(NULL));
  for (int i = 0; i < 110; i++) {
    for (int j = 0; j < 3; j++) {
      float x = (float)rand()/(float)(RAND_MAX/1);
      samples[i][j] = x;      
      //printf("%f\n", samples[i][j]);
    }
  }
}

void ReadData(float samples[][3]) {
  FILE* file_pointer;
  int read_pointer;
  file_pointer = fopen("1202_153833_train.txt", "r");
  for (int i = 0; i < 1200; i++) {
    read_pointer = fscanf(file_pointer, "%f %f %f\n", samples[i], samples[i] + 1, samples[i] + 2)    ;
    //printf("%f %f %f\n", samples[i][0], samples[i][1], samples[i][2]);
  }
}
