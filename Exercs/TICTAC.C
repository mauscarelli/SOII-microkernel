#include <system.h>
#include <stdio.h>
#include <conio.h>

PTR_DESC DMAIN, D_ESC, DA, DB;

void far ESCALADOR() {
    p_est->p_origem = D_ESC;
    p_est->p_destino = DA;
    p_est->num_vetor = 8;
    while (1) {
        iotransfer();
        disable();
        if (p_est->p_destino == DA)
            p_est->p_destino = DB;
        else p_est->p_destino = DA;
        enable();
    }
}

/* Processos exemplo */

void far COROTINAA() {
    while (1) printf("A");
}

void far COROTINAB() {
    while (1) printf("B");
}

main() {
    /* Cria processos */
    DA = cria_desc();
    DB = cria_desc();
    D_ESC = cria_desc();
    DMAIN = cria_desc();
    newprocess(COROTINAA, DA);
    newprocess(COROTINAB, DB);
    newprocess(ESCALADOR, D_ESC);

    /* Transfere controle para o escalador */
    transfer(DMAIN, D_ESC);
}

