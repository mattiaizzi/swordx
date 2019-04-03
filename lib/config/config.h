#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#include "../list/list.h"

struct Config{
  int alpha;
  int orderBy;
  int recursive;
  int follow_link;
  int min_length;
  List ignore;//parole da ignorare
  List explude;//file da non prendere in considerazione
  char* path;
  char* log;
};
typedef struct Config Config;

Config config_init();

void config_setLog(Config*, char*);

void config_setAlpha(Config*, int);

void config_setOrder(Config*, int);

void config_setRecursive(Config*, int);

void config_setFollow(Config*, int);

void config_setMinLength(Config*, int);

void config_setExplude(Config*, List*);

void config_setIgnore(Config*, List*);

void config_setPath(Config*, char*);

char* config_getPath(Config*);

char* config_getLog(Config*);

int config_getAlpha(Config*);

int config_getOrder(Config*);

int config_getRecursive(Config* );

int config_getFollow(Config* );

int config_getMinLength(Config* );

List config_getIgnore(Config* );

List config_getExplude(Config*);
#endif
