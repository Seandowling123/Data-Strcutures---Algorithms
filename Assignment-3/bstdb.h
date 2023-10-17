#ifndef BSTDB_H
#define BSTDB_H

int bstdb_init(void);
int bstdb_add(char *name, int word_count);
int bstdb_get_word_count(int doc_id);
char *bstdb_get_name(int doc_id);
void bstdb_stat(void);
void bstdb_quit(void);
struct Tree_Node *tree_search(struct Tree_Node *temp, int key, int start);
void check_balance(struct Tree_Node *temp);
void avg_search_time();
void total_nodes();
void max_search_time();

#endif