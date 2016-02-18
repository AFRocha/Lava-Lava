/*
 * @author AndréFRocha 
 * Fila para os carros
 */

#include <stdlib.h>
#include <stdio.h>
#include "fila_carros.h"


typedef struct no *NoPtr;

typedef struct no {
   Carro c;
   NoPtr prox;
} No;


typedef struct fil {
   int n_elem;
   NoPtr inicio;
   NoPtr fim;
} Fila_dinamic;

/***ERRO***/

static int erro = FILA_OK;

/***OPERACOES***/

/**
 * Cria a fila de carros
 */

Fila filac_criar( void )
{
  Fila f = (Fila) malloc( sizeof( Fila_dinamic ) );
  if ( f != NULL ) {
    f->n_elem = 0;
    f->inicio = NULL;
    f->fim = NULL;
    return f;
  }

  erro = FILA_MEM_ERROR;
  return NULL;
}

/**
 * elimina a fila
 * @param f a fila a avaliar
 */

void filac_destruir( Fila f )
{
  NoPtr no = f->inicio;
  NoPtr elim;

  while ( no != NULL ) {
    elim = no;
    no = no->prox;
    free( elim );
  }
  free( f );
}

/**
 * insere um carro na fila
 * @param f a fila a usar
 * @requires !fila_esta_vazia(f)
 */

void filac_entrar( Fila f, Carro c)
{
  NoPtr novo = (NoPtr) malloc( sizeof( No ) );
  
  if ( novo != NULL ) {
	
     novo->c.ordemChegada = c.ordemChegada;
     novo->c.minChegada= c.minChegada;
     novo->c.tipoLavagem = c.tipoLavagem;
     novo->prox=NULL;
    if ( f->fim != NULL ) 
      f->fim->prox = novo;
    f->fim = novo;
    if ( f->inicio == NULL ) 
      f->inicio = novo;
    (f->n_elem)++;
  }
  else 
    erro = FILA_MEM_ERROR;
}

/**
 * retira um carro da fila
 * @param f a fila a usar
 * @requires !fila_esta_vazia(f)
 */

void filac_sair( Fila f )
{
  NoPtr elim = f->inicio;
  f->inicio = (f->inicio)->prox;
  if ( f->inicio == NULL )
    f->fim = NULL;
  f->n_elem--;
  free( elim );
}

/**
 * carro que esta no inicio da fila
 * @param f a fila a usar 
 * @requires !fila_esta_vazia(f)
 */

int filac_primeiro( Fila f )
{
  return (f->inicio)->c.ordemChegada;
}

/**
 * determina se uma fila de carros esta vazia
 * @param f a fila a avaliar
 */

int filac_esta_vazia( Fila f )
{
  return f->inicio == NULL;
}

/**
 * devolve o numero de carros que se encontram na fila
 * @param f a fila a avaliar
 */

int filac_n_elems ( Fila f )
{
  if (f->inicio == NULL) {
	printf("0\n");
	return 0;	
  } 	

  NoPtr percorre = f->inicio; 

  int conta=1;
  percorre = percorre->prox;
   while ( percorre != NULL ){
        percorre = percorre->prox;
	conta++;
   }
	return conta;
}

/**
 * faz sair todos os carros da fila, porem, sem a eliminar
 * @param f a fila a usar
 * @requires !fila_esta_vazia(f)
 */

void filac_apagar ( Fila f ) 
{
  while(f->inicio != NULL) {
	filac_sair(f);
  }
}
  
/**
 * Faz desistir os carros cujos quais o seu limite de espera foi ultrapassado
 * @param f a fila a usar
 * @requires !fila_esta_vazia(f)
 */

int filac_desistir ( Fila f, int minutoActual, int traco)
{	
   
	

   NoPtr percorre = f->inicio; 
  
   while ( percorre != NULL ) {
    if ((minutoActual-(percorre->c.minChegada)) >= (10)){
		if ( traco == 1)
			printf("minuto     %d: O carro %d, desistiu\n",minutoActual, percorre->c.ordemChegada);
		filac_sair(f);
		return 1;
    percorre = percorre->prox;
    } else 
    percorre = percorre->prox; return 0;
   }
   
} 

/**
 * Imprime os carros que estao na fila no formato n_elems[ordem_de_chegada minuto_de_chegada;...;] 
 * @param f a fila a imprimir
 */

void filac_imprimir( Fila f)
{  
  NoPtr percorre = f->inicio;   
  printf( "\n\n%d[",filac_n_elems(f));

  if ( !filac_esta_vazia( f ) ){
    printf( "%d %d", percorre->c.ordemChegada, percorre->c.minChegada );
    percorre = percorre->prox;
  }
    
  while ( percorre != NULL ) {
    printf( "; %d %d", percorre->c.ordemChegada, percorre->c.minChegada);
    percorre = percorre->prox;
  }

  printf( "]" );
}

/**
 * Impressao do tipo de lavagem que cada carro na fila vai fazer
 * @param f a fila a usar
 * @requires !fila_esta_vazia(f)
 */

void filac_imprimir2(Fila f)
{

  NoPtr percorre = f->inicio; 
  printf("\n-----NOTA-----\n");

  if ( !filac_esta_vazia( f ) ){
	if (percorre->c.tipoLavagem==0){
          printf( "\nO %d carro vai fazer uma lavagem do tipo 0(Basica!)\n", percorre->c.ordemChegada );
	} 
	if (percorre->c.tipoLavagem==1){
  	  printf( "O %d carro vai fazer uma lavagem do tipo 1(Cera!)\n", percorre->c.ordemChegada );
	}
	if (percorre->c.tipoLavagem==2){
  	  printf( "O %d carro vai fazer uma lavagem do tipo 2(DELUXE!)\n", percorre->c.ordemChegada );
	} 
	if (percorre->c.tipoLavagem!=0 && percorre->c.tipoLavagem!=1 && percorre->c.tipoLavagem!=2) {
    	    printf("O tipo de lavagem do %d carro nao foi bem definida (nao e tipo 0, nem tipo 1, nem tipo 2)\n", percorre->c.ordemChegada); 
	}
  percorre = percorre->prox;
  
	  
  while ( percorre != NULL ) {
        if (percorre->c.tipoLavagem==0){
    	   printf( "O %d carro vai fazer uma lavagem do tipo 0(Basica!)\n", percorre->c.ordemChegada );
	}
	if (percorre->c.tipoLavagem==1){
    	   printf( "O %d carro vai fazer uma lavagem do tipo 1(Cera!)\n", percorre->c.ordemChegada );
	}
	if (percorre->c.tipoLavagem==2){
    	   printf( "O %d carro vai fazer uma lavagem do tipo 2(DELUXE!)\n", percorre->c.ordemChegada );
	} 
	if (percorre->c.tipoLavagem!=0 && percorre->c.tipoLavagem!=1 && percorre->c.tipoLavagem!=2) {
           printf("O tipo de lavagem do %d carro nao foi bem definida (nao e tipo 0, nem tipo 1, nem tipo 2)\n", percorre->c.ordemChegada); 
	}
    percorre = percorre->prox;
 	 }
   } printf("--------------");
}

/**devolve o erro associado as operacoes sobre filas**/

int filac_erro( void )
{
  return erro;
}

/**reinicia o erro associado as operacoes sobre filas**/

void filac_reset_erro( void )
{
  erro = 0;
}
