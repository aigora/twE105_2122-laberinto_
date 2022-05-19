#include <stdio.h>
#include <stdlib.h>


typedef struct Laberinto
{
    int largo;
    int ancho;
    int tamano;
    char* casillas;
}Laberinto;



int main()
{
    Laberinto laberinto;
    int filas = 1;
    int columnas = 1;
    int contador = 0;
    int i = 0;
    int j = 0;
    int primeraLinea = 0;
    char caracter;

   FILE* pf = NULL;

   pf = fopen("laberinto.txt", "r");


    if(pf != NULL)
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
        laberinto.tamano = filas * columnas;

        laberinto.casillas = (char*)malloc(laberinto.tamano);
        fclose(pf);
    }

   pf = fopen("laberinto.txt", "r");

    if(pf != NULL)
    {
        char valor;

        while(fscanf(pf, "%c", &valor) != EOF)
        {
            if(valor != '\n' && valor != ' ')
            {
                *(laberinto.casillas + contador) = valor;
                contador++;
            }

        }
        fclose(pf);
    }

    for(i = 0; i < laberinto.tamano; i++)
    {
        caracter = laberinto.casillas[i];

        if(caracter == '1')
        {
            laberinto.casillas[i] = '#';
        }
    }

    for(i = 0; i < laberinto.largo; i++)
    {
        for(j = 0; j < laberinto.ancho; j++)
        {
            int PosvMatriz = laberinto.ancho * i + j;

            printf("%c ", laberinto.casillas[PosvMatriz]);
        }
        printf("\n");
    }
    return 0;
}


