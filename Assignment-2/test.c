
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "t2.h"
#include "t1.h"

void fill_with_duplicates(int *array, int size)
{
  for (int i = 0; i < size; i++){
    array[i] = rand() % size;
  } 
}

void printArray(int* arr, int size){
  int i;
  for(i=0; i<size;i++){
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int ARRAY_SIZE =  60000;