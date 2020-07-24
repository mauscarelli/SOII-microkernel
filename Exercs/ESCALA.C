#include<System.h>
#include<stdio.h>
#include<conio.h>

/*** Estruturas ***/

/* Cada processo deve ter um descritor associado */
typedef struct desc_p {
    /* Nome do processo */
    char nome[35];

    /* Estado do processo */
    enum {
        ativo, terminado
    } estado;

    /* Ponteiro para descritor de contexto */
    PTR_DESC contexto;

    /* Ponteiro para o próximo descritor de processo */
    struct desc_p *prox_desc;
} DESCRITOR_PROC;

/* Ponteiro para a fila de processos */
typedef DESCRITOR_PROC *PTR_DESC_PROC;

typedef struct registros {
    unsigned bx1, es1;
} regis;

typedef union k {
    regis x;
    char far *y;
} APONTA_REG_CRIT;

/*** Variáveis globais ***/

PTR_DESC d_esc, d_main;

APONTA_REG_CRIT a;

PTR_DESC_PROC prim = NULL;

/*** Componentes e funções básicas do núcleo ***/

/* Associa um descritor de processo ao código do processo e coloca-o na fila dos prontos */
void far cria_processo(void far (*end_proc)(), char nome_p[35]) {
    PTR_DESC_PROC p, s;
    PTR_DESC d_p;

    /* Cria descritor (tipo DESCRITOR_PROC) dinamicamente */
    p = (DESCRITOR_PROC *) malloc(sizeof(DESCRITOR_PROC));

    if(p == NULL) {
        printf("\n\tMemoria Insuficiente para alocacao de processo\n");
        exit(1);
    }

    /* Copia (strcpy) nomep para nome */
    strcpy(p->nome,nome_p);

    /* Marca estado como "ativo" */
    p->estado = ativo;

    /* Inicia descritor de contexto */
    d_p = cria_desc();
    p->contexto = d_p;
    p->fila_sem = NULL;
    newprocess(end_proc,p->contexto);

    /* Insere descritor de processo na fila dos prontos */
    if(prim == NULL) {
        p->prox_desc = p;
        prim = p;
    } else {
        p->prox_desc = prim;
        s = prim;
        while(s->prox_desc != prim) s = s->prox_desc;
        s->prox_desc = p;
    }
}

/* Transfere o controle do programa principal (main) para o escalador */
void far dispara_sistema() {
    PTR_DESC desc_dispara;
    d_esc = cria_desc();
    desc_dispara = cria_desc();
    newprocess(escalador, d_esc);
    transfer(desc_dispara, d_esc);
}

/* Co-rotina o escalador */
void far escalador() {
    /* Inicia estrutura do iotransfer
     * Co-rotina chamadora: escalador
     */
    p_est->p_origem = d_esc;

    /* Co-rotina destino: prim->contexto - a primeira da fila dos prontos */
    p_est->p_destino = prim->contexto;

    /* Interrupção do timer */
    p_est->num_vetor = 8;

    /* Inicia ponteiro para R.C. do DOS */
    _AH = 0x34;
    _AL = 0x00;
    geninterrupt(0x21);
    a.x.bx1 = _BX;
    a.x.es1 = _ES;

    /* Loop principal (eterno) */
    while (1) {
        /* Chama iotransfer */
        iotransfer();

        /* Desabilita interrupções */
        disable();

        /* Se não está na R.C. troca o processo */
        if (!*a.y) {
            /* Procura pŕoximo processo ativo */
            prim = Procura_prox_ativo();

            /* Se não achou, volta para o DOS */
            if (prim == NULL) volta_DOS();

            /* Se achou, atribui ao cabeça dde fila e muda o destino do iotransfer */
            p_est->p_destino = prim->contexto;
        }

        /* Habilita interrupções */
        enable();
    }
}

/* Marca o processo chamador como "terminado" */
void far termina_processo() {
    /* Desabilita as interrupções */
    disable();

    /* Marca o processo corrente como terminado */
    prim->estado = terminado;

    /* Habilita as interrupções */
    enable();

    /* Laço eterno com comando nulo */
    while (1);
}

/*** Funções úteis ***/

/* Desinstala o escalador e retorna o controle para o DOS */
void far volta_DOS() {
    disable();
    setvect(8, p_est->int_anterior);
    enable();
    printf("\nVolta DOS\n");
    exit(0);
}

/*
 * Retorna o endereço do descritor do próximo processo ativo da fila dos prontos
 * (a partir da posição atual do cabeça da fila - "prim")
 */
PTR_DESC_PROC far Procura_prox_ativo() {
    PTR_DESC_PROC s;
    if (prim == NULL) return NULL;
    s = prim->prox_desc;

    /* Percorre a lista dos descritores dos processos,
     * a partir do próximo descritor após aquele apontado pelo PRIM,
     * à procura do primeiro com estado ativo
     */
    while (s->estado != ativo && s != prim)s = s->prox_desc;

    /* Se não achou ativo */
    if (s->estado != ativo) return NULL;

    /* Se achou, retorna o endereço do descritor do processo ativo */
    return s;
}

/* Processos exemplo */

void far COROTINAA() {
    int i = 2000;
    while (i--) {
        printf("A");
    }
}

void far COROTINAB() {
    int i = 4000;
    while (i--) {
        printf("B");
    }
}

void far COROTINAC() {
    int i = 5000;
    while (i--) {
        printf("A");
    }
}

void far COROTINAD() {
    int i = 3000;
    while (i--) {
        printf("B");
    }
}

main() {
    /* Cria processos */
    cria_processo(COROTINAA, "COROTINA A");
    cria_processo(COROTINAB, "COROTINA B");
    cria_processo(COROTINAC, "COROTINA C");
    cria_processo(COROTINAD, "COROTINA D");

    /* Transfere controle para o escalador */
    dispara_sistema();
}

