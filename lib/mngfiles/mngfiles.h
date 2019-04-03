#ifndef MNGFILES_H_INCLUDED
#define MNGFILES_H_INCLUDED
#include "../config/config.h"
#include <wchar.h>

void mngfiles_load(char**, int, Config*);
void mngfiles_end();
wchar_t* mngfiles_extractText(char*);


#endif
