/*Modulo che implementa un albero binario di ricerca bilanciato, permette di
mantenere un dizionario di tutte le stringhe aggiunte e, nel caso una stringa
sia già presente, ne aumenta il numero di occorrenze. L' albero gestisce stringhe
estese.*/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "avl.h"

Node*avl_createNode(const wchar_t*);
int avl_max(int,int);
int avl_getHeight(Node*);
int avl_getBF(Node*);
Node* avl_addNode(Node*, const wchar_t*);
void avl_toArrayNode(Node*, int*, Node*);
void avl_sort(Node*, int, int);
void avl_merge(Node*, int, int, int);
void avl_freeNode(Node*);
Node* avl_treeminimum(Node*);
Node* avl_deleteNode(Node*, const wchar_t*);
void avl_balance(Avl*);
Node* avl_balanceNode(Node*);
Node* avl_leftleft(Node* node);
Node* avl_leftright(Node* node);
Node* avl_rightleft(Node* node);
Node* avl_rightright(Node* node);

//inizializza albero bilanciato
Avl avl_init(){
  Avl avl = {NULL};
  return avl;
}

//crea un nodo foglia contenente una parola
Node*  avl_createNode(const wchar_t* string){
  Node* node = (Node*)malloc(sizeof(Node));
  if(node==NULL){
    exit(0);
  }
  node->string = string;
  node->occurrence = 1;
  node->right = NULL;
  node->left = NULL;
  return node;
}

/**
  * @desc cerca, a partire da un nodo, se è presente una parola
  * @param Node* $node - il nodo di partenza
  * @param const wchar_t* $string - la parola da cercare
  * @return Node* - restituisce il nodo se presente, NULL altrimenti
*/
Node* avl_search(Node* node, const wchar_t* string){
  if(node == NULL || wcscmp(string, node->string)==0){
    return node;
  }
  if(wcscmp(string, node->string)>0){
    return avl_search(node->right, string);
  }else{
    return avl_search(node->left, string);
  }
}

//funzione di servizio, restituisce il massimo tra due interi
int avl_max(int i,int j){
  if(i<j) return j;
  else return i;
}

//funzione di servizio, calcola l'altezza di un nodo
int avl_getHeight(Node* node){
  if(node==NULL) return 0;
  else return 1+avl_max(avl_getHeight(node->right),avl_getHeight(node->left));
}

//funzione di servizio, calcola il fattore di bilanciamento di un nodo
int avl_getBF(Node* node){
  if(node==NULL) return 0;
  else return (avl_getHeight(node->left) - avl_getHeight(node->right));
}

//funzione di servizio, aggiunge il nodo ad un albero
Node* avl_addNode(Node* node, const wchar_t* string){
  if(node==NULL){
    return avl_createNode(string);
  }else if(wcscmp(string, node->string)>0){
    node->right = avl_addNode(node->right, string);
  }else{
    node->left = avl_addNode(node->left, string);
  }
  return node;
}

/**
  * @desc aggiunge un nodo ad un albero e, se il nodo già esiste, incrementa
  *       il numro di occorrenze
  * @param Avl* $avl - l'albero a cui si vuole aggiungere il nodo
  * @param const wchar_t* $string - la parola contenuta nel nodo
*/
void avl_add(Avl* avl, const wchar_t* string){
  Node* node = avl_search(avl->root,string);
  if(node==NULL){
    avl->root = avl_addNode(avl->root, string);
    //bilanciamento albero
    avl_balance(avl);
  }else{
    node->occurrence++;
  }
}

/**
  * @desc stampa l'albero binario
  * @param Node* $node - il nodo di partenza
  * @param int $prof - valore intero per indentatura dell'albero
*/
void avl_print(int prof, Node* node){
  int spaces=4*prof;
  if(node!=NULL){
    avl_print(prof+1,node->left);
    printf("%*ls%ls\n",spaces,L" ", node->string);
    avl_print(prof+1,node->right);
  }
}

/**
  * @desc calcola il numero di parole memorizzate nell'albero
  * @param Node* $node - il nodo di partenza
  * @return int - restituisce il numero di elementi memorizzati
*/
int avl_size(Node* node){
  if(node==NULL) return 0;
  else return 1 +avl_size(node->left)+avl_size(node->right);
}

//funzione di servizio, ricorsivamente aggiunge il nodo all'array passato
void avl_toArrayNode(Node* array, int* index, Node* node){
  if(node!=NULL){
    avl_toArrayNode(array, index, node->left);
    array[*index] = *node;
    (*index)++;
    avl_toArrayNode(array, index, node->right);
  }
}

/**
  * @desc dato un albero, lo trasforma in array
  * @param Avl* $avl - l'albero da trasformare
  * @return Node* - restituisce i nodi ordinati in ordine alfabetico
*/
Node* avl_toArray(Avl* avl){
  if(avl->root==NULL){
    return NULL;
  }
  //alloco la memoria per memorizzare tutti i nodi dell'albero
  Node* array = (Node*)malloc(sizeof(Node)*avl_size(avl->root));
  int index = 0;
  avl_toArrayNode(array, &index, avl->root);
  return array;
}

/**
  * @desc dato un albero, lo trasforma in array
  * @param Avl* $avl - l'albero da trasformare
  * @return Node* - restituisce i nodi ordinati in base al numero di occorrenze
*/
Node* avl_toArrayOrderByOcc(Avl* avl){
  Node* array = avl_toArray(avl);
  if(array==NULL) return NULL;
  avl_sort(array,0 , avl_size(avl->root));
  return array;
}

//implementazione del merge sort per ordinare in base al numero di occorrenze
void avl_sort(Node* array, int left, int right ){
  if(right>left){
    int m = left+((right-left)/2);
    avl_sort(array, left, m);
    avl_sort(array, m+1, right);
    avl_merge(array, left, m, right);
  }
}


void avl_merge(Node* array, int left, int m, int right){
  int dim1 = m-left+1;
  int dim2 = right-m;
  Node array1[dim1];
  Node array2[dim2];
  int i=0;
  for(i=0; i<dim1; i++){
    array1[i] = array[left+i];
  }
  for(i=0; i<dim2;i++){
    array2[i] = array[m+i+1];
  }
  int j=0;
  i=0;
  int k=left;
  while(i<dim1 && j<dim2){
    if(array1[i].occurrence<array2[j].occurrence){
      array[k] = array1[i];
      i++;
    }else if(array2[j].occurrence<array1[i].occurrence){
      array[k]=array2[j];
      j++;
    }else{//se il numero di occorrenze è uguale ordino alfabeticamente
      if(wcscmp(array1[i].string,array2[j].string)<0){
        array[k]= array1[i];
        i++;
      }else{
        array[k]=array2[j];
        j++;
      }
    }
    k++;
  }
  while(i<dim1){
    array[k] = array1[i];
    k++;
    i++;
  }
  while(j<dim2){
    array[k] = array2[j];
    k++;
    j++;
  }
}

/**
  * @desc dealloca la memoria dell'albero
  * @param Avl* $avl - l'albero da eliminare
*/
void avl_free(Avl* avl){
  avl_freeNode(avl->root);
  avl->root = NULL;
}

//funzione di servizio per deallocare ricorsivamente ogni nodo
void avl_freeNode(Node* node){
  if(node != NULL){
    avl_freeNode(node->left);
    avl_freeNode(node->right);
    free(node);
  }
}

/**
  * @desc elimina una determinata parola all'interno dell'albero.
  * @param Avl* $avl - l'albero da cui si vuole eliminare il nodo
  * @param const wchar_t* $string - la parola da eliminare
*/
void avl_delete(Avl* avl, const wchar_t* string){
  if(avl->root!=NULL){
    avl->root = avl_deleteNode(avl->root, string);
    //bilancio l'albero
    avl_balance(avl);
  }
}

//funzione che ricorivamente cerca ed elimina il nodo se presente
Node* avl_deleteNode(Node* node, const wchar_t* string){
    if (node == NULL){
      return node;
    }
    //se la stringa da cancellare è più piccola bisogna eliminarla a sinistra
    if (wcscmp(node->string, string)<0){
      node->right = avl_deleteNode(node->right, string);
    }
    //se la stringa è più grande la cancello a destra
    else if(wcscmp(node->string, string)>0){
      node->left = avl_deleteNode(node->left, string);
    }
    else{
      //nodo foglia, lo elimino direttamente
      if((node->left)==NULL&&(node->right)==NULL){
        free(node);
        return NULL;
      }else if((node->left)!=NULL&&(node->right)!=NULL){//tutti e due i nodi presenti
        /*prendo il più piccolo elemento nel sottoalbero destro e lo sostituisco
        al nodo da cancellare*/
        Node* tmp = avl_treeminimum(node->right);
        node->string = tmp->string;
        /*ora devo cancellare il nodo che ho fatto salire dal sottoalbero destro;
        procedura ricorsiva fino a quando non raggiungo un nodo foglia*/
        node->right=avl_deleteNode(node->right,node->string);
      }else{
        //nodo con solamente un figlio, elimino il nodo e lo sostituisco con il figlio
        Node *tmp = ((node->right)!=NULL)?node->right:node->left;
        free(node);
        return tmp;
      }
    }
    return node;
}

//funzione di servizio, trova il valore minimo di un albero/sottoalbero
Node* avl_treeminimum(Node* node){
  Node *tmp= node;
  while((tmp->left)!=NULL){
    tmp =tmp->left;
  }
  return tmp;
}

//permette di bilanciare l'albero binario
void avl_balance(Avl *avl ) {
	avl->root = avl_balanceNode( avl->root );
}

//funzione che ricorsivamente bilancia ogni nodo
Node *avl_balanceNode(Node *node ) {
	Node *newroot = NULL;
	//bilanciamo i figli se esistono
	if( node->left ){
    node->left  = avl_balanceNode(node->left );
  }
	if( node->right ){
    node->right = avl_balanceNode(node->right);
  }
	int bf = avl_getBF(node);

	if( bf > 1 ) {
		//sbilanciato a sinistra
		if( avl_getBF(node->left) <= -1)//sbilanciamento sinistra destra
			newroot = avl_leftright( node );
		else//sbilanciamento sinistra sinistra
			newroot = avl_leftleft( node );

	} else if( bf < -1 ) {
		//sbilanciato  a destra
		if( avl_getBF( node->right )>= 1)//sbilanciamento destra sinistra
			newroot = avl_rightleft( node );
		else
			newroot = avl_rightright( node );//sbilanciamento destra destra
	} else {
    //bilanciato non cambia
		newroot = node;
	}
	return newroot;
}

//rotazione a destra : figlio sinistro nuova radice, sottoalbero destro attaccato a sinistra della vecchia radice
Node *avl_leftleft( Node *node ) {
 	Node *a = node;
	Node *b = a->left;
	a->left = b->right;
	b->right = a;
	return b;
}


//rotazione sinistra destra: prima rotazione a sinistra su figlio sinistro e dopo rotazione a destra
Node *avl_leftright( Node *node ) {
	Node *a = node;
	Node *b = a->left;
	Node *c = b->right;
	a->left = c->right;
	b->right = c->left;
	c->left = b;
	c->right = a;
	return c;
}

//simmetrico a sinistra destra
Node *avl_rightleft( Node *node ) {
	Node *a = node;
	Node *b = a->right;
	Node *c = b->left;
	a->right = c->left;
	b->left = c->right;
	c->right = b;
	c->left = a;
	return c;
}

//simmetrico a sinistra
Node *avl_rightright( Node *node ) {
	Node *a = node;
	Node *b = a->right;
	a->right = b->left;
	b->left = a;
	return b;
}
