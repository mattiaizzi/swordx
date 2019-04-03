/*modulo che si occupa di gestire i path e capire se si trattano di file, cartelle
oppure link per gestirli in modo corretto e produrre il file di output con il conteggio
delle occorrenze*/
#include <string.h>
#include <wchar.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <libgen.h>
#include "../list/list.h"
#include "../config/config.h"
#include "../ocount/ocount.h"
#include "../filters/filters.h"
#include "mngfiles.h"


void mngfiles_readFile(char*);
void mngfiles_loadWords(char*);
void mngfiles_readDir(char*);

List openFiles;
List expludeFiles;
Config* cnf;

/**
  * @desc gestisce i vari path passati gestendo link, file di testo e directory
  * @param char** $files - array di path
  * @param int $dim - dimensione dell'array files
  * @param Config* $config - file di configurazione delle option
*/
void mngfiles_load(char** files, int dim, Config* config){
  char programpath[500];
  //salvo il path della directory corrente
  if (getcwd(programpath, sizeof(programpath)) == NULL){
      perror("getcwd() error");
      exit;
  }
  cnf=config;
  //inizializzo la lista che terrà traccia dei file aperti
  openFiles = list_init();
  //setto la lista dei file che non sono da prendere in considerazione
  expludeFiles= config_getExplude(cnf);
  struct stat file;
  //preparo la funzione che mi permette di conteggiare le parole
  ocount_start();
  for(int i=0; i<dim ;i++){//analizzo tutti i file passati a argv[]
    //memorizzo in file i dati relativi al file puntato dal path
    int flag=lstat(files[i],&file);
    //se ci sono problemi con un path continuo con il successivo
    if(flag<0){
      printf("Errore: impossibile aprire %s\n", files[i]);
      continue;
    }
    if(S_ISDIR(file.st_mode)){
      //directory
      mngfiles_readDir(files[i]);
    }else if(S_ISLNK(file.st_mode)&&config_getFollow(cnf)==1){
      //link
      struct stat link;
      char buf[500];
      realpath(files[i], buf);
      lstat(buf, &link);
      if(S_ISDIR(link.st_mode)){
        mngfiles_readDir(buf);
      }else{
        mngfiles_readFile(buf);
      }

    }else if(S_ISREG(file.st_mode)){
      //file
      mngfiles_readFile(files[i]);
    }
    //imposto come directory corrente quella del programma
    chdir(programpath);
  }
}

//funzione ausiliara che si occupa dei file di testo
void mngfiles_readFile(char* path){
  //controllo che il file sia .txt
  regex_t regex;
  int match;
  match = regcomp(&regex, "^.*\\.txt$", REG_EXTENDED);
  if (match) {
      printf("Could not compile regex\n");
  }
  match = regexec(&regex, path, 0, NULL, 0);
  //se è un file di testo lo leggo e carico le parole, altrimenti non faccio nulla
  if (!match) {
      //apro il file e carico le parole
      mngfiles_loadWords(path);
  }else{
    printf("Non posso leggere %s\n",path );
  }
}

//funzione ausiliaria che controlla se il file è già stato letto, conteggia le parole
//e crea il file di log se necessario
void mngfiles_loadWords(char* path){
  //leggo il file solamente se non l'ho letto prima
  union UType readFile;
  struct stat statbuf;
  //salvo le informazioni del file
  stat(path,&statbuf);
  //controllo il device el'inode del file per verificare se è già stato letto
  readFile.file_id.device=statbuf.st_dev;
  readFile.file_id.inode=statbuf.st_ino;
  if(list_contains(&openFiles,&readFile,FILE_TYPE)){
    return;
  }
  int total=0;
  int ignore=0;
  struct timeval start, end;
  gettimeofday(&start, NULL);
  //estraggo il testo del file
  wchar_t* string = mngfiles_extractText(path);
  if(string){
    //carico le parole e conteggio quante parole sono state memorizzate
    total = ocount_load(string, config_getAlpha(cnf), config_getMinLength(cnf));
    //elimino le parole da ignorare e conteggio quante parole ho eliminato
    List list = config_getIgnore(cnf);
    Element *tmp = list.head;
    while(tmp!=NULL){
      wchar_t *string = tmp->value.w_string;
      ignore+=ocount_remove(string);
      tmp = tmp->next;
    }
    gettimeofday(&end, NULL);
    long unsigned time_spent = (end.tv_usec-start.tv_usec);
    //aggiungo il file a quelli già letti
    list_add(&openFiles, readFile, FILE_TYPE);
    //creo il file di log se presente nelle impostazioni
    if(strcmp(config_getLog(cnf),"")!=0){
      FILE *logfile = fopen(config_getLog(cnf),"a");
      char* filename = basename(path);
      fprintf(logfile,"\t%s \t%d \t%d \t%lu us\n",filename,total-ignore, ignore, time_spent);
      fclose(logfile);
    }
  }
}

//funzione ausiliaria che si occupa di gestire le directory
void mngfiles_readDir(char* path){
  DIR *dir;
  struct stat file;
  struct dirent *ent;
  if((dir=opendir(path)) != NULL){
    chdir(path);
    while ((ent = readdir(dir)) != NULL){
      lstat(ent->d_name,&file);//ritorna le informazioni del file e le salva file
      union UType c;
      c.string = ent->d_name;
      if(list_contains(&expludeFiles,&c,STRING_TYPE)){
        continue;
      }
      //leggo la directory solamente se c'è nelle opzioni
      if(S_ISDIR(file.st_mode)&&(config_getRecursive(cnf)==1)){
        if(strcmp(ent->d_name, ".") != 0  && strcmp(ent->d_name, "..") != 0){
          mngfiles_readDir(ent->d_name);
          chdir("..");
        }
      }else if(S_ISLNK(file.st_mode)&&config_getFollow(cnf)==1){
        //gestisco link
        struct stat link;
        char buf[500];
        realpath(ent->d_name, buf);
        lstat(buf, &link);
        if(S_ISDIR(link.st_mode)&&(config_getRecursive(cnf)==1)){
          mngfiles_readDir(buf);
        }else{
          mngfiles_readFile(buf);
        }
      }else if(S_ISREG(file.st_mode)){
        //gestisco file
        mngfiles_readFile(ent->d_name);
      }
    }
  }
  closedir(dir);
}


//termina la lettura dei path scrivendo i risultati nel file ti output
void mngfiles_end(){
  //elimino le parole
  Occurrence* occ = ocount_finalize(config_getOrder(cnf));
  FILE *dst;
  dst = fopen(config_getPath(cnf),"wb");
  if(dst == NULL)
  {
     printf("Error!");
     exit(0);
  }
  for(int i=0; i<ocount_getLength();i++){
    fwprintf(dst, L"%ls %d \r\n",occ[i].word,occ[i].n);
  }
  fflush(dst);
  fclose(dst);
}

//funzione ausiliaria che, dato un path di un file, ne estrae il testo per intero
wchar_t* mngfiles_extractText(char* path){
  FILE *file = fopen(path,"r");
  if (file == NULL) {
    perror("Failed: ");
    exit(0);
  }
  int length;
  char* buffer;
  fseek (file, 0, SEEK_END);
  length = ftell (file) + sizeof(char);
  fseek (file, 0, SEEK_SET);
  buffer = (char*)malloc(sizeof(char)*length);
  if(buffer){
    fread(buffer,1,length,file);
  }
  buffer[length-sizeof(char)]=0;
  wchar_t *pwcs = (wchar_t *)malloc( MB_CUR_MAX*strlen(buffer));
  mbstowcs( pwcs, buffer, MB_CUR_MAX*strlen(buffer));
  fclose(file);
  return pwcs;
}
