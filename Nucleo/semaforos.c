#include<Nucleo.h>

void far COROTINAA(){
   int i = 2000;
   while(i--){
     P(&s1);
     fprintf(teste,"A");
   }
   termina_processo();
}

void far COROTINAB(){
   int i = 2000;
   while(i--){
     fprintf(teste,"B");
     V(&s1);
   }
   termina_processo();
}

void far COROTINAC(){
   int i = 2000;
   while(i--){
     P(&s1);
     fprintf(teste,"C");
   }
   termina_processo();
}

 void far COROTINAD(){
   int i = 2000;
   while(i--){
     fprintf(teste,"D");
     V(&s1);
   }
   termina_processo();
}

main(){
   teste = fopen("teste.txt","w");
   inicia_semaforo(&s1,100);
   cria_processo(COROTINAA,"COROTINA A");
   cria_processo(COROTINAB,"COROTINA B");
   cria_processo(COROTINAC,"COROTINA C");
   cria_processo(COROTINAD,"COROTINA D");
   dispara_sistema();
}
