#include<System.h>
#include<stdio.h>
#include<conio.h>

typedef struct desc_p{
  char nome[35];
  enum{ativo,bloq_P,terminado} estado;
  PTR_DESC contexto;

  struct desc_p *fila_sem;

  struct desc_p *prox_desc;
} DESCRITOR_PROC;

typedef DESCRITOR_PROC *PTR_DESC_PROC;

typedef struct{
    int s;
    PTR_DESC_PROC Q;
} semaforo;

extern void far cria_processo(void far (*end_proc)(),char nome_p[35]);

extern void far termina_processo();

extern void far dispara_sistema();

extern void far inicia_semaforo(semaforo *sem, int n);

extern void far P(semaforo *sem);

extern void far V(semaforo *sem);

