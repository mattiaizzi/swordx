#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "avl.h"

int main(){
  setlocale(LC_CTYPE,"it_IT.UTF-8");
  Avl avl = avl_init();
  avl_add(&avl,L"A");
  avl_add(&avl,L"B");
  avl_add(&avl,L"C");
  avl_add(&avl,L"D");
  avl_add(&avl,L"E");
  avl_add(&avl,L"F");
  avl_add(&avl,L"G");
  avl_add(&avl,L"H");
  avl_add(&avl,L"I");
  avl_add(&avl,L"L");
  avl_add(&avl,L"M");
  avl_add(&avl,L"N");
  avl_add(&avl,L"O");
  avl_add(&avl,L"P");
  avl_add(&avl,L"Q");
  avl_add(&avl,L"R");
  avl_add(&avl,L"S");
  avl_add(&avl,L"T");
  avl_add(&avl,L"U");
  avl_add(&avl,L"V");
  avl_add(&avl,L"Z");
  avl_add(&avl,L"W");

  
  avl_print(0,avl.root);
  printf("\n\n");
  Node* array = avl_toArrayOrderByOcc(&avl);
  for(int i=0; i<avl_size(avl.root);i++){
    Node node = array[i];
    printf("%ls ", node.string);
  }
  avl_free(&avl);
  printf("\n");
  if(avl.root == NULL){
    printf("albero cancellato\n" );
  }




  //avl_print(avl.root);

}
