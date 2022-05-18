#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int largo;
    int ancho;
    int tamano;
    char* casillas;

}Laberinto; //Definicion de la estructura Laberinto


int main()
{
    //Declaracion de variables
    struct Laberinto laberinto;
    int filas = 1;
    int columnas = 1;
    int contador = 0;
    int i = 0, j = 0;
    int primeraLinea = 0;
    char caracter;

    /* Consideraciones: Si se quiere modificar el fichero "Laberinto.txt" para ampliar el laberinto existente en él,
    se deberán separar los caracteres correspondientes a pared, jugador y camino por espacios en blanco */

    FILE* pf = NULL; //Iniciamos el puntero a fichero pf, con NULL para asegurar que no apunte a nada

    pf = fopen("Laberinto.txt", "r");

    if(pf != NULL) //Bucle para leer los datos del fichero Laberinto.txt y almacenar los valores leidos en la estructura laberinto
    {
        char valor;

        while(fscanf(pf, "%c", &valor) != EOF)
        {
            if(valor == '\n')
            {
                filas++;
                primeraLinea = 1;
            }
            else if(valor == ' ' && primeraLinea == 0)
            {
                columnas++;
            }
        }

        laberinto.largo = filas;
        laberinto.ancho = columnas;
        laberinto.tamano = filas*columnas;

        laberinto.casillas = (*char)malloc(laberinto.tamano);

        fclose(pf); //Cerramos el fichero
    }

    pf = fopen("Laberinto.txt", "r");

    if(pf != NULL)
    {
        char valor;

        while(fscanf(pf, "%c", &valor) != EOF)
        {
            if(valor != '\n' && valor != ' ') //Bucle que lee el numero de casillas en el laberinto del fichero Laberinto.txt
            {
                *(laberinto.casillas + contador) = valor;
                contador++;
            }
        }
    }





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
