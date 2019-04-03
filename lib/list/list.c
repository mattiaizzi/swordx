#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "list.h"

void lista_deleteElement(Element**);
Element* list_createElement( union UType data, enum type_tag data_type, Element* );


List list_init(){
    List list = {NULL};
    return list;
}

int list_isEmpty(List* list){
    return (list->head)==NULL? 1:0;
}

Element* list_createElement(union UType data, enum type_tag data_type, Element* next){
  Element* new_element = (Element*)malloc(sizeof(Element));
  if(new_element==NULL){
      printf("Errore");
      exit(0);
  }
  new_element->data_type=data_type;
  new_element->next=next;
  new_element->value = data;
  return new_element;
}

void list_add(List* list, union UType data, enum type_tag data_type){
    list-> head = list_createElement(data, data_type, list->head);
}

void list_print(List* list){
    Element* tmp;
    for(tmp=list->head; tmp!=NULL; tmp=tmp->next){
        if(tmp->data_type==STRING_TYPE){
          printf("%s\n", tmp->value.string);
        }else if(tmp->data_type==WSTRING_TYPE){
          printf("%ls\n", tmp->value.w_string);
        }else{
          printf("inode: %ld -device: %ld\n",tmp->value.file_id.inode, tmp->value.file_id.device);
        }
    }
}

int list_size(List* list){
    int count=0;
    Element* tmp;
    for(tmp=list->head; tmp!=NULL; tmp=tmp->next){
        count++;
    }
    return count;
}

int list_contains(List* list, union UType *data, enum type_tag data_type){
    int flag=0;
    Element* tmp = list->head;
    if(tmp!=NULL && tmp->data_type==data_type){
      while(flag==0 && tmp!=NULL){
          if(data_type==WSTRING_TYPE&&wcscmp(tmp->value.w_string, data->w_string)==0)
              flag=1;
          else if(data_type==STRING_TYPE&&strcmp(tmp->value.string, data->string)==0)
              flag=1;
          else if(data_type==FILE_TYPE&& tmp->value.file_id.device==data->file_id.device && tmp->value.file_id.inode==data->file_id.inode )
              flag=1;
          tmp=tmp->next;
      }
    }
    return flag;
}


void list_deleteElement(Element** element){
    if((*element)->next!=NULL){
        list_deleteElement(&((*element)->next));
    }
    free(*element);
    *element=NULL;
}

void list_deletelist(List* list){
    if(list->head!=NULL){
        list_deleteElement(&(list->head));
    }
    list->head=NULL;
}
