#include <System.h>
#include <stdio.h>
#include <conio.h>

/*** Estruturas ***/

/* Cada processo deve ter um descritor associado */
typedef struct desc_p {
    /* Nome do processo */
    char nome[35];
    
    /* Estado do processo */
    enum {
        ativo, bloq_P, terminado
    } estado;
    
    /* Ponteiro para descritor de contexto */
    PTR_DESC contexto;

    /* Fila de processos bloquados por semáforo */
    struct desc_p *fila_sem;

    /* Ponteiro para o próximo descritor de processo */
    struct desc_p *prox_desc;
} DESCRITOR_PROC;

typedef DESCRITOR_PROC *PTR_DESC_PROC;

/* Semáforo geral (pode tomar qualquer valor inteiro não negativo) */
typedef struct {
    /* Prte inteira */
    int s;
    PTR_DESC_PROC Q;
} semaforo;

typedef struct registros {
    unsigned bx1, es1;
} regis;

typedef union k {
    regis x;
    char far *y;
} APONTA_REG_CRIT;

/*** Componentes e funções básicas do núcleo ***/

/* Associa um descritor de processo ao código do processo e coloca-o na fila dos prontos */
extern void far cria_processo(void far (*end_proc)(), char nome_p[35]);

/* Transfere o controle do programa principal (main) para o escalador */
extern void far dispara_sistema();

/* Co-rotina o escalador */
void far escalador();

/* Marca o processo chamador como "terminado" */
extern void far termina_processo();

/*** Funções úteis ***/

/* Desinstala o escalador e retorna o controle para o DOS */
void far volta_DOS();

/* Retorna o endereço de um processo */
void far procura_processo();

/*
 * Retorna o endereço do descritor do próximo processo ativo da fila dos prontos
 * (a partir da posição atual do cabeça da fila - "prim")
 */
PTR_DESC_PROC far Procura_prox_ativo();

/*** Primitivas para suporte dos semáforos ***/

/* Primitiva para iniciação o semáforo */
extern void far inicia_semaforo(semaforo *sem, int n);

/* Primitiva da operação P (ou Down) */
extern void far P(semaforo *sem);

/* Primitiva da operação V (ou Up) */
extern void far V(semaforo *sem);

