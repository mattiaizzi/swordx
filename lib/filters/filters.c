/*Modulo che permette di filtrare le stringhe e pulirle dai caratteri speciali
e dai segni di punteggiatura. Il modulo non riesce a gestire testi che hanno un
ritorno a capo*/
#include <wctype.h>
#include <wchar.h>
#include <stdlib.h>
#include "filters.h"

void filters_filterByLength(wchar_t*, int);
void filters_removeSpecial(int (*f)(wint_t),wchar_t*);
void filters_removeSpace(wchar_t*);
void filters_removePunct(wchar_t*);

/**
  * @desc filtra una stringa in ingresso eliminando segni di punteggiatura e caratteri
  *       speciali, ma è possibile eliminare anche le cifre e parole che non hanno
          una determinata lunghezza minima.
  * @param wchar_t* $string - stringa da filtrare.
  * @param int $mode - se impostato ad 1 considera solamente caratteri alfabetici,
                       alfanumerici altrimenti
  * @param int $length - lunghezza minima delle parole
*/
void filters_filter(wchar_t* string, int mode, int length){
    if(mode==1){
      //solo caratteri alfabetici
      filters_removeSpecial(iswalpha,string);
    }else{
      //caratteri alfanumerici
      filters_removeSpecial(iswalnum,string);
    }
    filters_removeSpace(string);
    if(length>0){
      filters_filterByLength(string, length);
    }
}

//funzione che elimina le parole che non hanno una determinata lunghezza minima
//si suppone che la stringa passata sia di sole parole divise da uno spazio
void filters_filterByLength(wchar_t* string, int dim){
  //puntatore per tenere traccia dei token
  wchar_t *ptr;
  //divido la stringa in parole
  wchar_t* token = wcstok(string,L" ",&ptr);
  wchar_t* dst = string;
  int length;
  while(token!=NULL){
    length=wcslen(token);
    if(length >= dim){
      //se la lunghezza è superiore al minimo, copio la parola nella stringa di
      //destinazione e aggiungo uno spazio alla fine
      for(int i=0; i < length; i++){
        *dst = token[i];
        dst++;
      }
      *dst=L' ';
      dst++;
    }
    token=wcstok(NULL,L" ",&ptr);
  }
  //tronco la stringa alla fine per evitare di avere caratteri aggiuntivi
  *dst=0;
}

//funzione che elimina i segni di punteggiatura, trasforma in minuscolo i caratteri
//e elimina i caratteri speciali.
//Prende in ingresso un puntatore a funzione perché è possibile filtrare caratteri
//alfanumerici o solamente alfabetici
void filters_removeSpecial(int (*f)(wint_t), wchar_t* string){
  while(*string){
    if (iswpunct(*string)){
      //se il carattere è un segno di punteggiatura lo sostituisco con uno spazio
       *string=L' ';
       string++;
    }else if (iswupper(*string)){
       //rendo il carattere piccolo e avanzo al carattere successivo
       *string = towlower(*string);
       string++;
    }else if(!((*f)((unsigned char)*string)||(*string) == L' ')){
        *string=L' ';
        string++;
    }else{
      string++;
    }
  }
  *string=0;
}

//funzione che rimuove gli spazi in eccesso
void filters_removeSpace(wchar_t* string){
  wchar_t *dst = string;
    while (*string){
        while (*string == L' ' && *(string+1) == L' '){
          string++;
        }
        *dst=*string;
        dst++;
        string++;
    }
    *dst = 0;
}
