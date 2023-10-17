#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 100
#define FILE_SIZE 150001

typedef struct Tree_Node {
	int count, key;
	char name[MAX_STRING_SIZE];
	struct Tree_Node *left, *right;
} Tree_Node;

struct Tree_Node *root;
int odd = 0;
int search = 0, num_searches = 0;

struct Tree_Node *tree_search(struct Tree_Node *temp, int key, int start);
void check_balance(struct Tree_Node *temp);
void avg_search_time();
void total_nodes();
void max_search_time();
int key_number = 0;
int array[FILE_SIZE];

void initialise_array() {
	for (int i = 0; i < FILE_SIZE; i++) {
		array[i] = 0;
	}
}

int generate_key() {
	int temp, index;
	key_number++;

	if (key_number == 1) {
		for (int i = 0; i < FILE_SIZE; i++) {
			if (i == FILE_SIZE / 2) {
				array[i] = FILE_SIZE + 1;
			}
			array[i] = i;
		}

		for (int i = 0; i < FILE_SIZE; i++) {
			index = rand() % FILE_SIZE;
			temp = array[i];
			array[i] = array[index];
			array[index] = temp;
		}

		return FILE_SIZE / 2;
	}

	else {
		return array[key_number];
	}
	// printf("%i ", key_number);
}

struct Tree_Node *new_node(char data[], int count, int key) {
	struct Tree_Node *temp = (Tree_Node *)malloc(sizeof(Tree_Node));
	strcpy(temp->name, data);
	temp->count = count;
	temp->left = NULL;
	temp->right = NULL;
	temp->key = key;

	return temp;
}

int bstdb_init(void) {
	// This function will run when the database first
	// starts. Function should return 1 if initialization
	// was successful and 0 if something went wrong.

	root = NULL;
	initialise_array();

	return 1;
}

struct Tree_Node *
insert(struct Tree_Node *temp, char data[], int count, int key) {
	if (temp == NULL) {
		return new_node(data, count, key);
	}

	else if (key > temp->key) {
		temp->right = insert(temp->right, data, count, key);
	}

	else {
		temp->left = insert(temp->left, data, count, key);
	}
	return temp;
}
int iteration = 0;

int bstdb_add(char *name, int word_count) {
	// Create a new node in the binary search tree,
	// populate it with the name and word_count of the arguments and store
	// the result in the tree.
	//
	// If something goes wrong and the data cannot be stored, this function
	// should return -1. Otherwise it should return the ID of the new node
	int key = generate_key();

	if (root == NULL) {
		root = new_node(name, word_count, key);
		return key;
	} else {
		insert(root, name, word_count, key);

		if (tree_search(root, key, 0)->key == key) {
			return key;
		} else {
			return -1;
		}
	}
}

int max_searches = 0;

struct Tree_Node *tree_search(struct Tree_Node *temp, int key, int start) {
	search++;
	start++;

	if (temp->key == root->key) {
		num_searches++;
	}

	if (start > max_searches) {
		max_searches = num_searches;
	}

	if (temp == NULL) {
		return NULL;
	}

	else if (temp->key == key) {
		return temp;
	}

	else if (key >= temp->key) {
		return tree_search(temp->right, key, start);
	}

	else {
		return tree_search(temp->left, key, start);
	}
}

int bstdb_get_word_count(int doc_id) {
	// Returns word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1

	if (tree_search(root, doc_id, 0) != NULL) {
		if (tree_search(root, doc_id, 0)->key == doc_id) {
			return tree_search(root, doc_id, 0)->count;
		} else {
			return -1;
		}
	} else {
		return -1;
	}
}

char *bstdb_get_name(int doc_id) {
	// Traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0

	if (tree_search(root, doc_id, 0) != NULL) {
		if (tree_search(root, doc_id, 0)->key == doc_id) {
			return tree_search(root, doc_id, 0)->name;
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
	return NULL;
}

void bstdb_stat(void) {
	// Perform some extra checks
	check_balance(root);
	avg_search_time();
	total_nodes();
	max_search_time();
}

void tree_delete(struct Tree_Node *temp) {
	if (temp->left != NULL) {
		tree_delete(temp->left);
	}

	else if (temp->right != NULL) {
		tree_delete(temp->right);
	}

	free(temp);
}

void bstdb_quit(void) {
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.

	tree_delete(root);
}

double countr;
double countl;

void counterr(struct Tree_Node *temp) {
	if (temp != NULL) {
		countr++;
		counterr(temp->right);
		counterr(temp->left);
	}
}

void counterl(struct Tree_Node *temp) {
	if (temp != NULL) {
		countl++;
		counterl(temp->right);
		counterl(temp->left);
	}
}

void check_balance(struct Tree_Node *temp) {
	countr = 0;
	countl = 0;

	counterr(temp->right);
	counterl(temp->left);

	double ratio = countr / countl;

	printf("Right to left size ratio     : %10.3f:1\n", ratio);
}

void total_nodes() {
	int total = countr + countl + 1;
	if (key_number == total) {
		printf("List size matches expected?  :            Y\n");
	} else {
		printf("List size matches expected?  :            N\n");
	}
}

void avg_search_time() {
	int avg = search / num_searches;
	printf("Avg nodes traversed          : %12i\n", avg);
}

void max_search_time() {
	printf("Max nodes traversed          : %12i\n", max_searches);
}
