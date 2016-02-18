/* 
 * @author AndréFRocha
 * Prototipos das funcoes
 * para a maquina de lavagem 
 * de carros
 */
#ifndef _Maquina
#define _Maquina

typedef struct maquina *Maquina;

Maquina maquina_criar( int a, int b,int c,int d, int e, int x, int g );
int maquina_destruir(Maquina maq);
int maquina_processar(Maquina maq);
int maquina_chegar(Maquina maq,int tipo);
int maquina_iniciar_lavagem(Maquina maq,int tipoLavagem);
void maquina_sair(Maquina maq,int tipo);
void maquina_printf(Maquina maq);
int maquina_erro( void );
void maquina_reset_erro( void );

#define MAQUINA_OK 0
#define MAQUINA_MEM_ERROR 1

#endif
