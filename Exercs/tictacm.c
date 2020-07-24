#include <system.h>
#include <stdio.h>
#include <conio.h>

PTR_DESC DMAIN, D_ESC, DA, DB;

void far ESCALADOR(){
   p_est->p_origem= D_ESC;
   p_est->p_destino = DA;
   p_est->num_vetor=8;
   while(1){
     iotransfer();
     disable(); 
     if(p_est->p_destino == DA)
       p_est->p_destino = DB;
     else p_est->p_destino = DA;
     enable();
   }
 }

 void far COROTINAA(){
   int i = 2000;
   while(i--){
     printf("A");
   }
 }

 void far COROTINAB(){
   int i = 2000;
   while(i--){
     printf("B");
   }
 }

 main(){
   DA = cria_desc();
   DB = cria_desc();
   D_ESC = cria_desc();
   DMAIN = cria_desc();
   newprocess(COROTINAA,DA);
   newprocess(COROTINAB,DB);
   newprocess(ESCALADOR,D_ESC);
   transfer(DMAIN,D_ESC);
 }
