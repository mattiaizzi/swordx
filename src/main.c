#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <glob.h>
#include "../lib/list/list.h"
#include "../lib/config/config.h"
#include "../lib/mngfiles/mngfiles.h"
#include "../lib/filters/filters.h"


int main(int argc, char* argv[]){
  //imposto locale per gestire in maniera ottimale i caratteri estesi
  setlocale(LC_CTYPE,"it_IT.UTF-8");
  Config cnf = config_init();
  List words = list_init();
  List files = list_init();
  //controllo se ci sono argomenti
  if(argc==1){
    printf("\nERRORE: <swordx [options] [inputs]>\n\nswordx -h per mostrare l'helper \n\n");
    return 0;
  }
  int i=1;
  //cerco le occorrenze di "-" per le varie options
  char* option = argv[i];
  while(option[0]=='-'){
    //helper
    if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"-help")==0){
      printf("%ls\n",mngfiles_extractText("src/helper.txt"));
      return 0;
    }
    //recursive
    else if(strcmp(argv[i],"-r")==0 || strcmp(argv[i],"-recursive")==0){
      config_setRecursive(&cnf,1);
    }
    //follow link
    else if(strcmp(argv[i],"-f")==0 || strcmp(argv[i],"-follow")==0){
      config_setFollow(&cnf,1);
    }
    //esclusione file
    else if(strcmp(argv[i],"-e")==0 || strcmp(argv[i],"-explude")==0){
      //carico il file da non prendere in considerazione
      i++;
      union UType string;
      string.string = argv[i];
      list_add(&files,string, STRING_TYPE);
    }
    //solo caratteri alfabetici
    else if(strcmp(argv[i],"-a")==0 || strcmp(argv[i],"-alpha")==0){
      config_setAlpha(&cnf,1);
    }
    //lunghezza minima delle parole da considerare
    else if(strcmp(argv[i],"-m")==0 || strcmp(argv[i],"-min")==0){
      i++;
      int n = atoi(argv[i]);
      config_setMinLength(&cnf,n);
    }
    //parole da ignorare
    else if(strcmp(argv[i],"-i")==0 || strcmp(argv[i],"-ignore")==0){
      //gestire le parole da ignorare caricate da file
      i++;
      wchar_t* string = mngfiles_extractText(argv[i]);
      wchar_t * token;
      wchar_t* ptr;
      filters_filter(string,0,0);
      token = wcstok(string, L" ",&ptr);
      while(token!=NULL){
        union UType word;
        word.w_string=token;
        list_add(&words, word, WSTRING_TYPE);
        token = wcstok(NULL, L" ",&ptr);
      }
    }
    //ordinare per numero di occorrenze
    else if(strcmp(argv[i],"-sortbyoccurency")==0 || strcmp(argv[i],"-s")==0){
      config_setOrder(&cnf,1);
    }
    //file di output
    else if(strcmp(argv[i],"-o")==0 || strcmp(argv[i],"-output")==0){
      i++;
      config_setPath(&cnf, argv[i]);
    }
    //file di log
    else if(strcmp(argv[i],"-l")==0 || strcmp(argv[i],"-log")==0){
      i++;
      union UType string;
      string.string = argv[i];
      list_add(&files, string, STRING_TYPE);
      config_setLog(&cnf, argv[i]);
    }
    //se l'opzione non è valida esco
    else{
      printf("Opzione non valida: %s\n\nswordx -h per mostrare l'helper\n\n",argv[i]);
      return 0;
    }
    i++;
    if(i>=argc){
      printf("\nERRORE: <swordx [options] [inputs]>\n\nswordx -h per mostrare l'helper \n\n");
      return 0;
    }
    option=argv[i];
  }
    int flags=0;
    glob_t results;
    int ret;
    //raggruppo tutti i risultati che rispettano la stringa(espressioni regolari)
    for (int j=i ; j < argc; j++) {
	    flags |= (j > i ? GLOB_APPEND : 0);
	    ret = glob(argv[j], flags, NULL, &results);
    }
    config_setExplude(&cnf, &files);
    config_setIgnore(&cnf, &words);
    mngfiles_load(results.gl_pathv, results.gl_pathc , &cnf);
    mngfiles_end();
}
