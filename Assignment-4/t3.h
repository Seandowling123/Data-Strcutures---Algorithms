#ifndef T3_H_
#define T3_H_

#define MAX_STRING_SIZE 200
#define MAX_VERTICES 10000

typedef struct Graph{
  int data[MAX_VERTICES][MAX_VERTICES];
} Graph;

typedef struct Vertex{
  int StopId;
  char Name[MAX_STRING_SIZE];
  double Latitude;
  double Longitude;
} Vertex;

typedef struct Edge{
  int from;
  int to;
  int weight;
} Edge;

typedef struct Distance{
    int from;
    int permanent;
    int distance;
} Distance;

int load_edges ( char *fname ); //loads the edges from the CSV file of name fname
int load_vertices ( char *fname );  //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory ( void ) ; // frees any memory that was used

#endif