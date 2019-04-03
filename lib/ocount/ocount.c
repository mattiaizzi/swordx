/*modulo che si occupa del conteggio delle occorrenze delle parole*/
#include <stdlib.h>
#include <wchar.h>
#include "ocount.h"
#include "../avl/avl.h"
#include "../filters/filters.h"
#include "../config/config.h"


Avl avl;
int ocount_length=0;

/**
  * @desc funzione di inizializzazione
*/
void ocount_start(){
  avl = avl_init();
}

/**
  * @desc carica un testo memorizzando le parole con le relative occorrenze
  * @param wchar_t* $string - testo da caricare
  * @param int $alpha - se messo a uno considera solo caratteri alfabetici, alfanumerici altrimenti
  * @param int $min - lunghezza minima delle parole da conteggiare
  *@return il numero di parole totali memorizzate
*/
int ocount_load(wchar_t* string,int alpha, int min){
  //devo filtare qui la stringa in base al file config
  int count=0;
  filters_filter(string, alpha, min);
  wchar_t * token;
  wchar_t* ptr;
  token = wcstok(string, L" ",&ptr);
  while(token!=NULL){
    avl_add(&avl, token);
    count++;
    token = wcstok(NULL, L" ",&ptr);
  }
  return count;
}

/**
  * @desc rimuove una parola dal conteggio
  * @param char* $word - parola da cancellare
  * @return il numero di occorrenze della parola eliminata
*/
int ocount_remove(wchar_t* word){
  int count=0;
  Node* tmp =avl_search(avl.root,word);
  if(tmp!=NULL){
    count=tmp->occurrence;
  }
  avl_delete(&avl,word);
  return count;
}

/**
  * @desc crea un array ordinato alfabeticamente o per numero di occorrenze delle parole
  * @param OrderBy $order - ORDERBYOCC per numero di occorrenze ORDERBYNAME alfabeticamente
  * @return un array ordinato di Occurrence
*/
Occurrence* ocount_finalize(OrderBy order){
  ocount_length = avl_size(avl.root);
  Occurrence* occurrences = (Occurrence*)malloc(sizeof(Occurrence)*ocount_length);
  Node* nodes;
  if(order==ORDERBYOCC){
    nodes = avl_toArrayOrderByOcc(&avl);
  }else{
    nodes = avl_toArray(&avl);
  }
  for(int i=0 ; i<ocount_length ; i++){
    Occurrence occurrence;
    occurrence.word = nodes[i].string;
    occurrence.n = nodes[i].occurrence;
    occurrences[i] = occurrence;
  }
  return occurrences;
}

/**
  * @return la lunghezza dell'array ordinato
*/
int ocount_getLength(){
  return ocount_length;
}
