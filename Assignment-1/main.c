
#include<stdio.h>
#include<stdlib.h>
#include <string.h> //for strcpy and strcmp
#include <ctype.h>  //for isalnum

// NB: this skeleton is here to help you - ignore anything that does not help you
// There are a lot of others (and better!) ways of doing this

// note this sekeleton uses a global variable. It therefore will only work with 
// one hashtable, of the size set. In general global variables should be avoided
// as their global scope makes it very challenging to see where they are updated 
// (as it can be anywhere!), and don't work well when several threads/programs
// update them at the same time (concurency), amongst others problems.
// Having said that, they can make coding a bit easier, so that's why we are using 
// one here. If you can, try using a hashTable as a variable, passing it as a 
// parameter to functions!



#define MAX_STRING_SIZE 20 //max length of a string
#define ARRAY_SIZE 59  //best be prime

typedef struct Element Element;
struct Element{
    char name[MAX_STRING_SIZE];
    int count;
};

Element* hashTable[ARRAY_SIZE];
int collisions = 0;

int hash_function(char* s){
    int hash = 0;
    while(*s){
        hash = hash + *s;
        s++;
    }
    hash = hash % ARRAY_SIZE;
    return hash;
}

int hash3(char* s){
  int hash = 0;
  while(*s){
    hash = 1 + (hash + *s) % (ARRAY_SIZE-1);
    s++;
  }
  return hash;
}


// returns the element with name name or NULL if the element is not present
Element* search (char* name){
  int key = hash_function(name);
  int key2 = hash3(name);
  int key3 = 0;

  for (int i = 0; i < ARRAY_SIZE; i++){
    if (strcmp(hashTable[i]->name, name) == 0){

      return hashTable[i];

    }
  }

  return NULL;
}

int count() {
  int num = 1;
  for (int i = 0; i < ARRAY_SIZE; i++){
    if (hashTable[i]->name[0] != 0){
      num++;
    }
  }
  return num;
}

void insert(char* name){

  int key = hash_function(name);
  int string_collision = 0;
  int increment = 0;
  char zeros[MAX_STRING_SIZE];
  memset(zeros,0,MAX_STRING_SIZE);

  while (strcmp(hashTable[key]->name, zeros) != 0){
    
    collisions++;
    string_collision++;
    increment = string_collision * hash3(name);

    if ((key + increment) >= ARRAY_SIZE) {
      key = (key + increment) % ARRAY_SIZE;
    }
    else {
      key = key + increment;
    }
  }

  strcpy(hashTable[key]->name, name);
  hashTable[key]->count = 1;

}

//searches the name in the array, if it is there increment its count, if not, add it
void addOrIncrement(char* name){
  
  int found = 0;
  for (int i = 0; i < ARRAY_SIZE; i++){

    if (strcmp(search(hashTable[i]->name)->name, name) == 0) {
      hashTable[i]->count++;
      found = 1;
    }
  }

  if (found == 0) {
      insert(name);
  }
}

// prints the number of occurences, or 0 if not in the file
void printNumberOfOccurences(char* name){

  if (strcmp(search(name)->name, name) == 0) {
    printf(">>> %s - %i \n", search(name)->name, search(name)->count);
  }
  else {
    printf("%s not in table", name);
  }
}

// function from the assignment 0
// Reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
void next_token ( char *buf, FILE *f, int string_max ) {
	// start by skipping any characters we're not interested in
	buf[0] = fgetc(f);
	while ( !isalnum(buf[0]) && !feof(f) ) { buf[0] = fgetc(f); }
	// read string of alphanumeric characters
	int i=1;
	for (;;) {
		buf[i] = fgetc(f);                // get next character from file
		if( !isalnum(buf[i]) ) { break; } // only load letters and numbers
		if( feof(f) ) { break; }          // file ended?
		if( i < (string_max-1) ) { ++i; } // truncate strings that are too long
	}
	buf[i] = '\0'; // NULL terminate the string
}


//  Reads the contents of a file and adds them to the hash table - returns 1 if file was successfully read and 0 if not.
int load_file ( char *fname ) {
	FILE *f;
	char buf[MAX_STRING_SIZE];

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
  else {
    	printf("File %s loaded\n", fname);
  }
	
	// read until the end of the file
	while ( !feof(f) ) {
		next_token(buf, f, MAX_STRING_SIZE);
		addOrIncrement(buf);
	}

	fclose(f);

	return 1;
}


int main( int argc, char **argv )
{
  char check_name[MAX_STRING_SIZE];
  memset(check_name,0,MAX_STRING_SIZE);
  int found = 0;

  for (int i = 0; i < ARRAY_SIZE; i++) {
    struct Element *new_Element = malloc(sizeof(struct Element));
    hashTable[i] = new_Element;
    new_Element->count = 0;
  }

  load_file("names.csv");
  //load_file(argv[1]);

  printf(" Capacity: %i\n", ARRAY_SIZE);
  printf(" Num Terms: %i\n", count());
  printf(" Collisions: %i\n", collisions);

  double counter = count();
  double size = ARRAY_SIZE;
  double load = counter/size;

  printf("Load %.2f\n", load);

  printf("Enter term to get frequency or type \"quit\" to escape\n"); 

  while (strcmp(check_name, "quit") != 0) {

    found = 0;
    
    scanf("%s", check_name);

    struct Element *check_element = malloc(sizeof(struct Element));
    strcpy(check_element->name, check_name);

    for (int i = 0; i < ARRAY_SIZE; i++){

      if (strcmp(search(hashTable[i]->name)->name, check_element->name) == 0) {
        found++;
      }
    }

    if (found > 0 && strcmp(check_element->name, "quit") != 0) {
      printNumberOfOccurences(check_name);
    }
    else if (strcmp(check_element->name, "quit") != 0) {
      printf(">>> %s - 0\n", check_name);
    }

  }
  
  for (int i = 0; i < ARRAY_SIZE; i++){
    printNumberOfOccurences(hashTable[i]->name);
  }
    return 0;
}
