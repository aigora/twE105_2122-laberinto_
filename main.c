#include <stdio.h>
#include <stdlib.h>

#define LONGITUD_ARCHIVO_MAX 40

typedef struct Laberinto
{
    int largo;
    int ancho;
    int tamano;
    char* casillas;
}Laberinto;

///Prototipos de funciones
void DibujarLaberinto(Laberinto laberinto);



int main()
{
    /*
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
    */
    Laberinto laberinto;

    LeerArchivo();

    DibujarLaberinto();

    return 0;
}

int RellenarCasillas(const char* archivo, Laberinto* laberinto) ///Funcion que rellena las casillas del laberinto a partir de los valores leidos
{
    int contador = 0;

    FILE* pf = fopen(archivo, "r"); //Abre el archivo introducido por teclado

    if(pf != NULL)
    {
        char valor;

        while(fscanf(pf, "%c", &valor) != EOF) //Lee caracter a caracter el archivo
        {
            if(valor != '\n' && valor != ' ') //Si no es un salto de línea o un espacio, entonces es un caracter
            {
                laberinto->casillas[contador] = valor; //Almacena el valor leido del archivo en la casilla correspondiente de la estructura Laberinto
                contador++;
            }

        }

        return 1; //Lectura correcta
    }


    return 0; //Error al abrir el archivo
}

int LeerArchivo(const char* archivo, Laberinto* laberinto) ///Funcion para leer el contenido del archivo
{
    int primeraLinea = 0;
    int filas = 1;
    int columnas = 1;

    FILE* pf = NULL;
    pf = fopen(archivo, "r");

    if(pf != NULL)
    {
        char valor;


        while(fscanf(pf, "%c", &valor) != EOF) //Lee caracter a caracter y lo guarda en "valor"
        {
            if(valor == '\n') //Cada nueva linea, se incrementa el numero de filas
            {
                filas++;
                primeraLinea = 1;
            }

            else if(valor == ' ' && primeraLinea == 0) //Cada espacio, se incrementa el numero de columnas
            {
                columnas++;
            }
        }

        //Parametros del laberinto

        laberinto->largo = filas;
        laberinto->ancho = columnas;
        laberinto->dimensiones = filas * columnas;


        laberinto->casillas = (char*)malloc(laberinto->dimensiones); //Reserva memoria para la variable casillas

        fclose(pf);

        return RellenarCasillas(archivo, laberinto);

        //Si el archivo se ha leido correctamente, RellenarCasillas = 1 y al llamar a la funcion el contenido de laberinto queda modificado
    }

    //Error, no se pudo abrir el fichero

    return 0;
}


void DibujarLaberinto(Laberinto laberinto) ///Funcion para imprimir por pantalla el laberinto
{
    int i = 0;
    int j = 0;


    for(i = 0; i < laberinto.largo; i++) //Bucles anidados para recorrer el laberinto
    {
        for(j = 0; j < laberinto.ancho; j++)
        {
            int PosvMatriz = laberinto.ancho * i + j; //Vectoriza la matriz que contiene al laberinto

            printf("%c ", laberinto.casillas[PosvMatriz]); //Muestra la casilla separada por un espacio (para ver mas limpio el laberinto)
        }
        //Siguiente fila
        printf("\n");
    }
}
