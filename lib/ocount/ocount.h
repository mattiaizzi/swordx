#ifndef OCOUNT_H_INCLUDED
#define OCOUNT_H_INCLUDED
#define ORDERBYNAME 0
#define ORDERBYOCC 1
#include <wchar.h>

typedef int OrderBy;

struct Occurrence{
  const wchar_t* word;
  int n;
};

typedef struct Occurrence Occurrence;

void ocount_start();

int ocount_load(wchar_t*, int, int);

Occurrence* ocount_finalize(OrderBy);

int ocount_remove(wchar_t*);

int ocount_getLength();


#endif
