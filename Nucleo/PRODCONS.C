#include<stdio.h>
#include<nucleo.h>

#define MAX 20;

/* 
   Para contextualizar o problema do produtor consumidor apresentamos um cenário de pandemia,
   Uma loja deve operar com no máximo 20 clientes simultâneamente na loja
   Caso a loja esteja lotada nenhum cliente novo pode entrar
   Caso a loja esteja vazia logicamente nenhum cliente sai da loja
*/

semaforo mutex;
semaforo cheio;
semaforo vazio;

FILE *teste;

void far entra_cliente(){
   int i = 2000;
   while(i--){
     P(&vazio);
     P(&mutex);
     fprintf(teste,"Cliente entrou na loja.\nCapacidade de novos clientes: %d\n",vazio.s);
     V(&mutex);
     V(&cheio);
   }
   termina_processo();
}

void far sai_cliente(){
   int i = 2000;
   while(i--){
     P(&cheio);
     P(&mutex);
     fprintf(teste,"Cliente saiu da loja.\nClientes ainda presentes: %d\n",cheio.s);
     V(&mutex);
     V(&vazio);
   }
   termina_processo();
}


main(){
   teste = fopen("C:/teste.txt","w");
   inicia_semaforo(&mutex,1);
   inicia_semaforo(&cheio,0);
   inicia_semaforo(&vazio,20);
   cria_processo(entra_cliente,"Entra Cliente");
   cria_processo(sai_cliente,"Sai Cliente");
   dispara_sistema();
   fclose(teste);
}
