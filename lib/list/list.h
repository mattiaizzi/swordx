#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <wchar.h>
#include <sys/types.h>

struct FileId{
    dev_t device;
    ino_t inode;
};

typedef struct  Element Element;

union UType{
  char* string;
  wchar_t* w_string;
  struct FileId file_id;
};


enum type_tag {STRING_TYPE, WSTRING_TYPE, FILE_TYPE};

struct Element{
  enum type_tag data_type;
    union UType value;
    struct Element *next;
};
typedef struct  Element Element;

struct List{
    Element* head;
};
typedef struct List List;

List list_init();

int list_isEmpty(List*);

void list_add(List*, union UType, enum type_tag);

void list_print(List*);

int list_size(List*);

int list_contains(List*, union UType*, enum type_tag);

void list_deleteList(List* );





#endif // LISTA_H_INCLUDED
