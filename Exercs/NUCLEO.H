#include<System.h>
#include<stdio.h>
#include<conio.h>


extern void far cria_processo(void far (*end_proc)(),char nome_p[35]);

extern void far termina_processo();

extern void far dispara_sistema();