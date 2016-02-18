/*
 * @author AndréFRocha 
 * Máquina para lavagem de carros
 */
#include <stdlib.h>
#include <stdio.h>
#include "maquina.h"
#include "fila_carros.h"
#include "gerador.h"

/*
 *Estrutura maquina, com uma fila de carros e parametros que 
 *definem os lucros, o cenário de chegadas e o tempo da simulacao e o traco
 */

typedef struct maquina{ 
   Fila f;
   int lucro0,lucro1,lucro2,limInfChegadas,limSupChegadas,tempoSimulacao,traco;
} maquina;

static int erro = MAQUINA_OK; 

int contaCarros = 0; /*Representa todos os carros que vao entrar no sistema*/
int relogio = 1; /*Relogio que a cada minuto ira' controlar a simulacao*/
int lucroTotal = -200; /*Lucro total da empresa lavalava*/
int nLavagensT0 = 0; /*Numero lavagens tipo 0*/
int nLavagensT1 = 0; /*Numero lavagens tipo 1*/
int nLavagensT2 = 0; /*Numero lavagens tipo 2*/
int finalLavagem = 0;/*Agendamento do min em que a maquina estara' disponivel*/
int carro; /*Identificador do carro actual na maquina*/
int desistencias = 0; /*Numero total de desistencias que ira' haver*/
int tipoLav = 0; /*Identificador da lavagem que a maquina esta a fazer*/
int houveDesistencia=0; /*Se houve e quantas desistencias no minuto actual*/
int tipo = 0; /*Tipo de lavagem que ira' ser gerada pelo gerador*/
int tempoEspera = 0; /*Tempo total de espera dos carros atendidos*/
int tempo = 0; /*tempo eh o tempo restante para a chegada do proximo carro*/

/* Criacao da maquina */

Maquina maquina_criar(int a,int b,int c,int d,int e,int x,int g){
   Maquina maq = (Maquina) malloc( sizeof( maquina ) );
   if ( maq != NULL ) {
      maq->lucro0 = a;
      maq->lucro1 = b;
      maq->lucro2 = c;
      maq->limInfChegadas = d;
      maq->limSupChegadas = e;
      maq->tempoSimulacao = x;
      maq->traco = g;
      return maq;
   }
   erro = MAQUINA_MEM_ERROR;
   return NULL;
}

int maquina_destruir(Maquina maq){
   filac_destruir(maq->f);
   free(maq);
}

/*Efectua o processamento da maquina, i.e.
 *Depois do primeiro carro chegar 'a fila,
 *entra na maquina e e' gerado um minuto
 *para quando a maquina estara' vazia 
 *consoante o tipo de lavagem do carro
 *que la entrou.
 *Nota: A cada minuto que passa no relogio(ciclo),
 *a funcao ira' averiguar se chega um carro a fila,
 *se chega, eh gerado um novo minuto da proxima 
 *chegada, e assim sucessivamente.
 *Apos a saida do ciclo(fim do tempo simulacao)
 *caso a fila ainda conter carros, estes ja' nao desistem
 *e enquanto a fila nao estiver vazia, a maquina continuara'
 *a aceitar lavagens, acabando sempre por fechar com um carro
 *a sair da maquina.
 */
int maquina_processar(Maquina maq){
   maq->f = filac_criar();
   tempo = geraInteiro(maq->limInfChegadas, maq->limSupChegadas); 
   finalLavagem = relogio;
   while(relogio < (60 * maq->tempoSimulacao)){ 
      tipo = geraTipoLavagem();
      if (relogio == tempo){
         maquina_chegar(maq, tipo);
	 tempo = tempo + geraInteiro(maq->limInfChegadas, maq->limSupChegadas);
      }
      if (finalLavagem > 1 && relogio == finalLavagem) 	
         maquina_sair(maq,tipo);

      else if (relogio < finalLavagem) 
         tempoEspera++;

      else if (relogio >= finalLavagem && !filac_esta_vazia(maq->f))
         maquina_iniciar_lavagem(maq, tipo);
		
      if (filac_desistir(maq->f,relogio, maq->traco) == 1){
         carro = carro + 1;
       	 desistencias = desistencias +1;
	 houveDesistencia++;
      } 
	
      else 
         carro = carro;
	
      relogio++; 
   }

   while (!filac_esta_vazia(maq->f)){
      if (finalLavagem>1 && relogio==finalLavagem) 	
         maquina_sair(maq,tipo);
      if (relogio >= finalLavagem && !filac_esta_vazia(maq->f))
         maquina_iniciar_lavagem(maq, tipo);
      relogio++;
   }
   relogio = finalLavagem; 	
   maquina_sair(maq,tipo);
}

/*Funcao que processa a chegada de um carro
 *'a fila. Ele e' adicionado a' fila de carros,
 *e' incrementado o valor do numero total de carros
 *que entraram no sistema, e se o traco for 1,
 *imprime o carro e o minuto que este entrou na fila
 */

int maquina_chegar(Maquina maq,int tipo){	
   Carro c;
   c.ordemChegada = contaCarros + 1;
   c.minChegada = relogio;
   filac_entrar(maq->f, c);
   contaCarros++;
   if ( maq->traco == 1)
      printf("minuto     %d: Carro %d lavagem %d entra na fila\n", 
      relogio, contaCarros, tipo);
}

/*Funcao que inicia uma lavagem cujo tipo foi gerado aleatoriamente,
 *e gera um minuto no qual a maquina ira' estar novamente disponivel. 
 *Incrementa  o lucroTotal consoante o lucro de cada lavagem.
 *Incrementa também o numero do tipo lavagens que foram realizadas.
 *Caso o traco for 1, imprime qual o carro que entrou na maquina e 
 *a que minuto. 
 */

int maquina_iniciar_lavagem(Maquina maq,int tipoLavagem){
   if (maq->traco == 1){
      if(tipoLavagem == 0){
         printf("minuto     %d: Carro %d lavagem %d entra na maquina\n",
         relogio, carro + 1, tipoLavagem);
	 finalLavagem = relogio + 5;
	 lucroTotal = lucroTotal + maq->lucro0;
	 nLavagensT0++;
	 tipoLav = 0;
	 filac_sair(maq->f);
      } 

      if(tipoLavagem==1){
         printf("minuto     %d: Carro %d lavagem %d entra na maquina\n",
         relogio, carro + 1, tipoLavagem);
	 finalLavagem = relogio + 7;	
	 lucroTotal=lucroTotal + maq->lucro1;
	 nLavagensT1++;
	 tipoLav = 1;
	 filac_sair(maq->f);	                
      } 

      if(tipoLavagem==2){ 
         printf("minuto     %d: Carro %d lavagem %d entra na maquina\n",
         relogio, carro + 1, tipoLavagem);
	 finalLavagem = relogio+10; 
	 lucroTotal=lucroTotal + maq->lucro2;
	 nLavagensT2++;
	 tipoLav = 2;
	 filac_sair(maq->f);
      }carro++;

   } else {
      if(tipoLavagem==0){
         finalLavagem = relogio+5; 
	 lucroTotal = lucroTotal+ maq->lucro0;
	 nLavagensT0++;
	 filac_sair(maq->f);	
      } 

      if(tipoLavagem==1){
         finalLavagem = relogio+7;
       	 lucroTotal=lucroTotal+ maq->lucro1;
  	 nLavagensT1++;
	 filac_sair(maq->f);
      } 

      if(tipoLavagem==2){ 
         finalLavagem = relogio+10;  		
	 lucroTotal=lucroTotal+ maq->lucro2;
	 nLavagensT2++;
	 filac_sair(maq->f);
      }
   }
}

/*Saida de um carro da maquina, se traco
 *for 1, imprime que carro saiu e a que minuto*/

void maquina_sair(Maquina maq,int tipo){
   if (maq->traco == 1)
      printf("minuto     %d: Carro %d lavagem %d sai da maquina\n", 
      relogio, carro - houveDesistencia, tipoLav);
   houveDesistencia = 0; 
}

/*Impressao do Relatorio*/

void maquina_printf(Maquina maq){  

double tempoMedioAtendidos = ((double)((nLavagensT0*5)+
(nLavagensT1*7)+(nLavagensT2*10)+tempoEspera)/contaCarros);

   printf("\nRelatorio\n");
   printf("Minuto actual: %d\n", relogio);
   printf("Conteudo da fila: carros[]\n");
   printf("Lucro: %d\n", lucroTotal);
   printf("Numero de carros que entraram no sistema: %d\n", contaCarros);
   printf("Numero de carros atendidos: %d\n",
   nLavagensT0+nLavagensT1+nLavagensT2);
   printf("Numero de lavagens atendidas por tipo0: %d, 1: %d, 2: %d \n",
   nLavagensT0,nLavagensT1,nLavagensT2);
   printf("Numero de carros que desistiram: %d\n", desistencias);
   printf("Tempo medio no sistema: %f\n",(double)(((nLavagensT0*5+nLavagensT1*7
   +nLavagensT2*10)/(nLavagensT0+nLavagensT1+nLavagensT2))+tempoMedioAtendidos));
   printf("Tempo medio de espera dos atendidos: %f\n",tempoMedioAtendidos);
}

int maquina_erro(void){
  return erro;
}

void maquina_reset_erro(void){
  erro = 0;
}
