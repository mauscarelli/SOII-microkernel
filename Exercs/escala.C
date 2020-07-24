#include<System.h>
#include<stdio.h>
#include<conio.h>


PTR_DESC d_esc,d_main;


typedef struct registros{
   unsigned bx1,es1;
}regis;

typedef union k{
   regis x;
   char far *y;
}APONTA_REG_CRIT;

APONTA_REG_CRIT a;

typedef struct desc_p{
  char nome[35];
  enum{ativo,terminado} estado;
  PTR_DESC contexto;

  struct desc_p *prox_desc;
} DESCRITOR_PROC;

typedef DESCRITOR_PROC *PTR_DESC_PROC;

PTR_DESC_PROC prim = NULL;

void far volta_DOS(){
   disable();
   setvect(8,p_est->int_anterior);
   enable();
   printf("\nVolta DOS\n");
   exit(0);
}

PTR_DESC_PROC far Procura_prox_ativo(){
   PTR_DESC_PROC s;
   if(prim == NULL) return NULL;
   s = prim->prox_desc;
   while(s->estado != ativo && s != prim)
      s = s->prox_desc;
   if(s->estado != ativo)
      return NULL;
   return s;
}

void far cria_processo(void far (*end_proc)(),char nome_p[35])
{
   PTR_DESC_PROC p,s;
   PTR_DESC d_p;
   if((p=(DESCRITOR_PROC*)malloc(sizeof(DESCRITOR_PROC)))==NULL){
      printf("\n\tMemoria Insuficiente para alocacao de processo\n");
      exit(1);
   }
   strcpy(p->nome,nome_p);
   p->estado = ativo;
   d_p = cria_desc();
   p->contexto = d_p;
   p->fila_sem = NULL;
   newprocess(end_proc,p->contexto);
   if(prim == NULL){
      p->prox_desc = p;
      prim = p;
   }else{
      p->prox_desc = prim;
      s = prim;
      while(s->prox_desc != prim){
         s = s->prox_desc;
      }
      s->prox_desc = p;
   }
}

void far termina_processo(){
   disable();
   prim->estado = terminado;
   enable();
   while(1);
}


void far escalador(){
   p_est->p_origem = d_esc;
   p_est->p_destino = prim->contexto;
   p_est->num_vetor = 8;
   _AH=0x34;
   _AL=0x00;
   geninterrupt(0x21);
   a.x.bx1 = _BX;
   a.x.es1 = _ES;
   while(1){
     iotransfer();
     disable();
     if(!*a.y){
        prim = Procura_prox_ativo();
        if(prim == NULL)
           volta_DOS();
        p_est->p_destino = prim->contexto;
     }
     enable();
   }
}

void far dispara_sistema(){
   PTR_DESC desc_dispara;
   d_esc = cria_desc();
   desc_dispara = cria_desc();
   newprocess(escalador,d_esc);
   transfer(desc_dispara,d_esc);
}

void far COROTINAA(){
   int i = 2000;
   while(i--){
     printf("A");
   }
 }

 void far COROTINAB(){
   int i = 4000;
   while(i--){
     printf("B");
   }
 }

 void far COROTINAC(){
   int i = 5000;
   while(i--){
     printf("A");
   }
 }

 void far COROTINAD(){
   int i = 3000;
   while(i--){
     printf("B");
   }
 }

main(){
   cria_processo(COROTINAA,"COROTINA A");
   cria_processo(COROTINAB,"COROTINA B");
   cria_processo(COROTINAC,"COROTINA C");
   cria_processo(COROTINAD,"COROTINA D");
   dispara_sistema();
}

