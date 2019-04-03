#include "config.h"
#include "string.h"
#include "../list/list.h"



Config config_init(){
  Config cnf;
  cnf.alpha=0;
  cnf.orderBy=0;
  cnf.recursive=0;
  cnf.follow_link=0;
  cnf.min_length=0;
  cnf.ignore = list_init();
  cnf.explude= list_init();
  cnf.path="swordx.out";
  cnf.log="";
  return cnf;
}

void config_setLog(Config* cnf, char* path){
  cnf->log = path;
}

char* config_getLog(Config* cnf){
  return cnf->log;
}

void config_setAlpha(Config *cnf, int alpha){
  if(alpha==1){
    cnf->alpha=alpha;
  }
}

void config_setOrder(Config *cnf, int orderBy){
  if(orderBy==1){
    cnf->orderBy=orderBy;
  }
}

void config_setRecursive(Config *cnf, int recursive){
  if(recursive==1){
    cnf->recursive=recursive;
  }
}

void config_setFollow(Config *cnf, int follow){
  if(follow==1){
    cnf->follow_link=follow;
  }
}

void config_setMinLength(Config *cnf, int length){
  if(length>=0){
    cnf->min_length=length;
  }
}

void config_setExplude(Config *cnf, List *list){
  cnf->explude = *list;
}

void config_setIgnore(Config *cnf, List *list){
  cnf->ignore = *list;
}

void config_setPath(Config *cnf, char* path){
  cnf->path = path;
}

char* config_getPath(Config *cnf ){
  return cnf->path;
}

int config_getAlpha(Config *cnf ){
  return cnf->alpha;
}

int config_getOrder(Config *cnf ){
  return cnf->orderBy;
}

int config_getRecursive(Config *cnf ){
  return cnf->recursive;
}

int config_getFollow(Config *cnf){
  return cnf->follow_link;
}

int config_getMinLength(Config *cnf ){
  return cnf->min_length;
}

List config_getIgnore(Config *cnf ){
  return cnf->ignore;
}

List config_getExplude(Config *cnf ){
  return cnf->explude;
}
