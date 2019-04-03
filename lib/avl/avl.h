#ifndef AVL_H_INCLUDED
#define AVL_H_INCLUDED
#include <wchar.h>

struct Node{
  const wchar_t * string;
  int occurrence;
  struct Node *right, *left;
};

typedef struct Node Node;

struct Avl{
  Node* root;
};

typedef struct Avl Avl;

Avl avl_init();

void avl_add(Avl*, const wchar_t*);

void avl_delete(Avl*, const wchar_t*);

Node* avl_search(Node*, const wchar_t*);

int avl_size(Node*);

Node* avl_toArray(Avl*);

Node* avl_toArrayOrderByOcc(Avl*);

void avl_print(int, Node*);

void avl_free(Avl*);


#endif
