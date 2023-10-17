#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "t3.h"

#define MAX_STRING_SIZE 200
#define ARRAY_SIZE 10000
int index_edge = 0, index_vertex = 0;

struct Edge* edges[MAX_VERTICES];
struct Vertex* vertices[MAX_VERTICES];
struct Distance* distance[MAX_VERTICES];

void next_field( char *buf, FILE *f, int max ) {
	int i=0, quoted=0;
	
	for(;;) {	
		buf[i] = fgetc(f);
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		if(buf[i]==',' && !quoted) { break; }
		if(feof(f) || buf[i]=='\n') { break; }
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
}

int load_edges ( char *fname ){
  FILE *f;
	char buf[MAX_STRING_SIZE];
  int field = 0;
  f = fopen(fname, "r");

  for (int i = 0; i < ARRAY_SIZE; i++) {
    struct Edge *new_edge = malloc(sizeof(struct Edge));
    edges[i] = new_edge;
  }

  for (int i = 0; i < 3; i++){
    next_field(buf, f, MAX_STRING_SIZE);
  } 

  while ( !feof(f) ) {
    next_field(buf, f, MAX_STRING_SIZE);
    field++;

    if (field == 1){
      edges[index_edge]->to = atoi(buf);
      //printf("to: %i ", edges[index_edge]->to);
    }

    if (field == 2){
      edges[index_edge]->from = atoi(buf);
      //printf("from: %i ", edges[index_edge]->from);
    }

    if (field == 3){
      edges[index_edge]->weight = atoi(buf);
      //printf("wieght: %i \n", edges[index_edge]->weight);
      index_edge++;
      field = 0;
    }
  }
  printf("Loaded %i edges\n", index_edge);
  return 1;
}

int load_vertices ( char *fname ){
  FILE *f;
	char buf[MAX_STRING_SIZE];
  int field = 0, index = 0;
  f = fopen(fname, "r");

  for (int i = 0; i < ARRAY_SIZE; i++) {
    struct Vertex *new_vertex = malloc(sizeof(struct Vertex));
    vertices[i] = new_vertex;
  }

  for (int i = 0; i < 4; i++){
    next_field(buf, f, MAX_STRING_SIZE);
  }

  while ( !feof(f) ) {
    next_field(buf, f, MAX_STRING_SIZE);
    field++;

    if (field == 1){
      index = atoi(buf);
      vertices[index]->StopId = atoi(buf);
      //printf("StopId: %i ", vertices[index_vertex]->StopId);
    }

    if (field == 2){
      strcpy(vertices[index]->Name, buf);
      //printf("Name: %s ", vertices[index_vertex]->Name);
    }

    if (field == 3){
      vertices[index]->Latitude = atof(buf);
      //printf("Latitude: %f ", vertices[index_vertex]->Latitude);
    }

    if (field == 4){
      vertices[index]->Longitude = atof(buf);
      //printf("Longitude: %f \n", vertices[index_vertex]->Longitude);
      index_vertex++;
      field = 0;
    }
  }
  printf("Loaded %i vertices\n", index_vertex);
  return 1;
}

void initialise_distance(){
  for (int i = 0; i < MAX_VERTICES; i++){
    distance[i] = malloc(sizeof(struct Distance));
    distance[i]->distance = 0;
    distance[i]->permanent = 0;
  }
}

Graph* create_graph() {
  struct Graph* new_graph = malloc(sizeof(struct Graph));
  for (int i = 0; i < index_vertex; i++){
    for (int j = 0; j < index_vertex; j++){
      new_graph->data[i][j] = 0;
    }
  }
  return new_graph;
}

void add_edge(Graph *g, int from, int to, int weight) {
  g->data[to][from] = weight;
  g->data[from][to] = weight;
  //printf("%i %i %i\n", from, to, g->data[to][from]);
}

void reverseArray(int arr[], int start, int end){
  int temp;
  while (start < end)
  {
    temp = arr[start];  
    arr[start] = arr[end];
    arr[end] = temp;
    start++;
    end--;
  }  
} 

void print_distances(int origin, int to){
  int path[ARRAY_SIZE];
  int i = to, j = 0;
  while (i != origin){
    path[j] = i;
    i = distance[i]->from;
    //printf("%i ", i);
    j++;
  }
  path[j+1] = 0;

  reverseArray(path, 0, j);
  i = 1;

  printf("%i %s\n", vertices[origin]->StopId, vertices[origin]->Name);

  while (path[i] != to){
    printf("%i %s\n", vertices[path[i]]->StopId, vertices[path[i]]->Name);
    i++;
  }
  printf("%i %s\n", vertices[to]->StopId, vertices[to]->Name);
}

int shortest_distance(){
  int smallest = 100000;
  int index = 0;
  for (int i = 0; i < MAX_VERTICES; i++){
    if (distance[i]->distance < smallest && distance[i]->distance != 0 && distance[i]->permanent != 1){
      smallest = distance[i]->distance;
      index = i;
    }
  }
  return index;
}

void dijkstra(Graph* g, int origin, int endNode){
  initialise_distance();
  distance[origin]->permanent = 1;
  int permanent = origin;

  while (permanent != endNode){
    for (int i = 0; i < MAX_VERTICES; i++){

      if (g->data[permanent][i] != 0){

        if (distance[i]->distance == 0){
          distance[i]->distance = g->data[permanent][i] + distance[permanent]->distance;
          distance[i]->from = permanent;
        }
        
        else if (g->data[permanent][i] + distance[permanent]->distance < distance[i]->distance){
          distance[i]->distance = g->data[permanent][i] + distance[permanent]->distance;
          distance[i]->from = permanent;
        }
      }
    }
    permanent = shortest_distance();
    distance[shortest_distance()]->permanent = 1;
  }

  distance[origin]->distance = 0;
  print_distances(origin, endNode);
}

void shortest_path(int startNode, int endNode){
  Graph *graph = create_graph();

  for (int i = 0; i < index_edge; i++){
    add_edge(graph, edges[i]->to, edges[i]->from, edges[i]->weight);
  }

  dijkstra(graph, startNode, endNode);

}

void free_memory(void){
  //free(edges);
  //free(vertices);
  //free(distance);
}