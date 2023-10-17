#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "t2.h"
#include "t1.h"

int number_comparisons = 0;
int number_swaps = 0;
#define MAX_STRING_SIZE 200
#define ARRAY_SIZE 18626

typedef struct element element;
struct element{
  char title[MAX_STRING_SIZE];
  char platform[MAX_STRING_SIZE];
  int score;
  int year;
};

element* game[ARRAY_SIZE];

void printScores(struct element *arr[], int size){
  int i;
  for(i=0; i<size;i++){
    printf("%d ", arr[i]->score);
  }
  printf("\n");
}

void printField(struct element *arr[], int size){
  int i;
  for(i = (size - 10); i < size; i++){
    printf("%-51s", arr[i]->title);
    printf("%-21s", arr[i]->platform);
    printf("%-3d", arr[i]->score);
    printf("%d", arr[i]->year);
    printf("\n");
  }
  printf("\n");
}

void selectionSort(struct element *arr[], int size) 
{ 
  
  for (int i = 0; i < size; i++){

    struct element *smallest = malloc(sizeof(struct element));
    smallest->score = size - 1;

    for (int j = i; j < size; j++){

      if (game[j]->score < smallest->score){
        smallest = game[j];
        game[j] = game[i];
        game[i] = smallest;
      }

      if (j == size){
        free(smallest);
      }
    }
  }
}

void insertionSort(int arr[], int size) 
{

  for (int i = 0; i < size - 1; i++) {

    for (int j = i + 1; j > 0; j--) {
      
      if (arr[j - 1] > arr[j]) {
        int temp = arr[j];
        arr[j] = arr[j - 1];
        arr[j - 1] = temp;
      }

      else {
         break;
      }
    }
  }
}

void quickSort(struct element *arr[], int size)
{ 
  if (size > 1){
    int pivot = game[size - 1]->score;
    int a = 0, b = 1, prev_a = - 1, prev_b = size;

    while (a < b){

      for (int i = prev_a + 1; i < size; i++){

        if (game[i]->score > pivot){
          a = i;
          prev_a = i;
          break;
        }
        else if (i == size - 1){
          a = i;
          break;
        }
      }

      for (int i = prev_b - 1; i > 0; i--){

        if (game[i]->score < pivot){
          b = i;
          prev_b = i;
          break;
        }
        else if (i == 1){
          b = i;
          break;
        }
      }

      if (a < b){
        int temp = game[a]->score;
        game[a]->score = game[b]->score;
        game[b]->score = temp;
      }
    }

    int size_a = a, size_b = size - a;
    quickSort(game, size_b);
    quickSort(game, size_a);
  }
}

void next_field( char *buf, FILE *f, int max ) {
	int i=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
}

int load_file ( char *fname ){

  FILE *f;
	char buf[MAX_STRING_SIZE];
  int field = 0, index = 0, buf_int;
  f = fopen(fname, "r");

  for (int i = 0; i < ARRAY_SIZE; i++) {
    struct element *new_Element = malloc(sizeof(struct element));
    game[i] = new_Element;
  }

  while ( !feof(f) ) {
    next_field(buf, f, MAX_STRING_SIZE);
    field++;

    if (field == 1){
      strcpy(game[index]->title, buf);
      //printf("name %s ll\n", game[index]->title);
    }

    if (field == 2){
      strcpy(game[index]->platform, buf);
      //printf("platform %s ll\n", game[index]->platform);
    }

    if (field == 3){
      buf_int = atoi(buf);
      game[index]->score = buf_int;
      //printf("score %i ll\n", game[index]->score);
    }

    if (field == 4){
      buf_int = atoi(buf);
      game[index]->year = buf_int;
      //printf("year %i ll\n", game[index]->year);
      index++;
      field = 0;
    }
  }

  return 0;
}

int main(void) {

  load_file("t4_ign.csv");
  selectionSort(game, ARRAY_SIZE);
  printField(game, ARRAY_SIZE);
  return 0;

}