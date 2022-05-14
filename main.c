#include <stdio.h>

void imprime(char[11][11], int nFil, int nCol);
void movimiento(char tecla);

int main()
{
    //Declaracion de variables
    int i;
    int x = 3, y = 4; //Coordenadas del punto de partida "o" (columna 3, fila 4 de la matriz, la primera fila y columna son la 0)
    int xe = 9, ye = 6;

    //se podrian emplear estructuras para almacenar las coordenadas

    char laberintoPrueba[11][11] = {
    "XXXXXXXXXX",
    "XXXXXXXXXX",
    "XXX    XXX",
    "XXX XX XXX",
    "XXXoXX XXX",
    "XX  XX  XX",
    "XX XXXX  E",
    "XX    XXXX",
    "XXXXX    X",
    "XXXXXXXXXX"};

    imprime(laberintoPrueba, 10, 10);





    return 0;
}

void imprime(char matriz[11][11], int nFil, int nCol)
{
    int i, j;
    for(i=0; i<=nFil; i++)
    {
        for(j=0; j<=nCol; j++)
            printf("%c", matriz[i][j]);
        printf("\n");
    }
}

void movimiento(char tecla)
{
    do
    {

    } while(x != xe && y != ye);
}
