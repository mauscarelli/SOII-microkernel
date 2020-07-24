#include<stdio.h>
#include<nucleo.h>

semaforo s1;

void far COROTINAA(){
   int i = 2000;
   while(i--){
     P(&s1);
     printf("A");
   }
   termina_processo();
}

void far COROTINAB(){
   int i = 2000;
   while(i--){
     printf("B");
     V(&s1);
   }
   termina_processo();
}

void far COROTINAC(){
   int i = 2000;
   while(i--){
     P(&s1);
     printf("C");
   }
   termina_processo();
}

 void far COROTINAD(){
   int i = 2000;
   while(i--){
     printf("D");
     V(&s1);
   }
   termina_processo();
}

main(){
   inicia_semaforo(&s1,100);
   cria_processo(COROTINAA,"COROTINA A");
   cria_processo(COROTINAB,"COROTINA B");
   cria_processo(COROTINAC,"COROTINA C");
   cria_processo(COROTINAD,"COROTINA D");
   dispara_sistema();
}
