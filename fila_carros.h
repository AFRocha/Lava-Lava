/*
 * @author AndréFRocha 
 * Funções para a fila de carros
 */

#ifndef _FILA
#define _FILA

#include "carro.h"

typedef struct fil *Fila;

Fila filac_criar( void );

void filac_destruir( Fila f );

void filac_entrar( Fila f, Carro c);

void filac_sair( Fila f );

int filac_primeiro( Fila f );

int filac_esta_vazia( Fila f );

int filac_n_elems ( Fila f );

void filac_apagar ( Fila f ); 

int filac_desistir ( Fila f, int minutoActual, int traco); 

void filac_imprimir( Fila f); 

void filac_imprimir2(Fila f);

int filac_erro( void );

void filac_reset_erro( void );

#define FILA_OK 0

#define FILA_MEM_ERROR 1

#endif

