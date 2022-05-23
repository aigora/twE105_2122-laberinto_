#include <stdio.h>
#include <stdlib.h> //Para el uso de la funcion malloc()
#include <ctype.h> //Para el uso de la funcion toupper()

#define LONGITUD_ARCHIVO_MAX 40
#define SALIDA 1
#define VALIDO 2
#define PARED 0

typedef struct
{
    int largo;
    int ancho;
    int dimensiones;
    char* casillas;
}Laberinto;

typedef struct
{
    int x;
    int y;
}Jugador;

///Prototipos de funciones
void DibujarLaberinto(Laberinto laberinto, Jugador* jugador, int indices); //Indices enumera filas y columnas si es 1, si es 0 no las enumera
int LeerArchivo(const char* archivo, Laberinto* laberinto);
int RellenarCasillas(const char* archivo, Laberinto* laberinto);
void PedirArchivo(Laberinto* laberinto);
int ActualizarPosicion(Laberinto laberinto, Jugador* jugador, int x, int y);
int PedirMovimiento(Laberinto laberinto, Jugador* jugador);
void Ejecutar(Laberinto laberinto, Jugador* jugador);
void PedirUbicacionJugador(Laberinto laberinto, Jugador* jugador);



int main()
{
    Laberinto laberinto;
    Jugador jugador;

    PedirArchivo(&laberinto);

    DibujarLaberinto(laberinto, NULL, 1);

    PedirUbicacionJugador(laberinto, &jugador);

    DibujarLaberinto(laberinto, &jugador, 0);

    Ejecutar(laberinto, &jugador);

    free(laberinto.casillas); //Se libera la memoria reservada con malloc

    return 0;
}

void PedirUbicacionJugador(Laberinto laberinto, Jugador* jugador)
{
    int x;
    int y;

    int indice;
    int posicionCorrecta = 0;

    do
    {
        printf("Introduzca la posicion X del jugador: ");
        scanf("%d", &x);

        printf("Introduzca la posicion Y del jugador: ");
        scanf("%d", &y);

        indice = laberinto.ancho * y + x;

        if(x < 0 || x >= laberinto.ancho || y < 0 || y >= laberinto.largo)
        {
            printf("La posicion seleccionada no es valida!\n");
        }
        else if(laberinto.casillas[indice] == '#')
        {
            printf("Pared! Posicion no valida\n");
        }
        else
        {
            posicionCorrecta = 1;
            jugador->x = x; //Se guardan las coords iniciales en la estructura Jugador
            jugador->y = y;
        }
    }
    while(posicionCorrecta != 1);
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

void PedirArchivo(Laberinto* laberinto) ///Funcion para pedir el archivo
{
    int resultadoLectura = 0;

    char archivo[LONGITUD_ARCHIVO_MAX];

    printf("Introduzca el archivo del laberinto: ");


    do //Pide el archivo hasta que se introduzca uno valido
    {
        scanf("%s", archivo); ///El nombre del archivo introducido por teclado debe ser <nombrearchivo>.<txt>
        resultadoLectura = LeerArchivo(archivo, laberinto); //La funcion LeerArchivo devuelve 0 en caso de no poder leer el archivo

        if(resultadoLectura == 0) //Si no ha LeerArchivo no ha podido leer el archivo es porque hemos introducido un nombre no valido
        {
            printf("El archivo no es valido, introduzca uno valido: ");
        }
    }while(resultadoLectura != 1);
}

void DibujarLaberinto(Laberinto laberinto, Jugador* jugador, int indices) ///Funcion para imprimir por pantalla el laberinto
{
    int i = 0;
    int j = 0;
    int posicionJugador;

    if(jugador != NULL)
    {
        posicionJugador = laberinto.ancho * jugador->y + jugador->x; //Convierte las coords 2D a 1D del jugador, "vectoriza" a la "matriz jugador"
    }

    if(indices == 1) //Si indices es 1 (TRUE) entonces enumera las filas y columnas del laberinto para facilitar la colocacion del jugador
    {


        printf("   ");
        for(i = 0; i < laberinto.ancho; i++)
        {
            printf("%d ", i);
        }

        printf("\n");
    }

    for(i = 0; i < laberinto.largo; i++) //Bucles anidados para recorrer el laberinto
    {
        if(indices == 1)
        {
            printf("%d  ", i);
        }

        for(j = 0; j < laberinto.ancho; j++)
        {
            int PosvMatriz = laberinto.ancho * i + j; //Vectoriza la matriz que contiene al laberinto

            if(jugador != NULL && PosvMatriz == posicionJugador) //Si las coordenadas 1D de la matriz y el jugador coinciden y no contienen una pared, entonces imprime al jugador
            {
                printf("X "); //La X simboliza al jugador
            }
            else
            {
                printf("%c ", laberinto.casillas[PosvMatriz]); //Mostrar casilla
            }
        }
        //Siguiente fila
        printf("\n");
    }

    printf("\n");
}

int ActualizarPosicion(Laberinto laberinto, Jugador* jugador, int x, int y) ///Funcion para mover al jugador
{
    int indice = laberinto.ancho * y + x;

    if(x <= -1 || x >= laberinto.ancho || y <= -1 || y >= laberinto.largo)
    {
        return SALIDA; //Si las nuevas coordenadas se salen de los limites del laberinto, retorna SALIDA
    }
    else if(laberinto.casillas[indice] == '#')
    {
        printf("Ahi hay una pared!\n"); //Comprueba que las nuevas coordenadas del jugador en el laberinto no corresponde a una pared
        return PARED;
    }
    else //Si las nuevas coordenadas no llevan al jugador a una pared o a la salida, entonces se guardan las nuevas coordenadas, para registrar el movimiento
    {
        jugador->x = x;
        jugador->y = y;
        return VALIDO;
    }

}

int PedirMovimiento(Laberinto laberinto, Jugador* jugador) ///Funcion para pedir al usuario que introduzca un movimiento
{
    char letra;
    int x = jugador->x;
    int y = jugador->y;
    int movimientoValido = 0;
    char min2mayus;
    do
    {
    printf("Introduzca: W, A, S, D para mover al jugador\n");
    scanf("%s", &letra);
    min2mayus = toupper(letra); //La funcion toupper() convierte un caracter en minusculas a mayusculas

    switch(min2mayus)
    {
    case 'W':
        y--;
        movimientoValido = ActualizarPosicion(laberinto, jugador, x, y);
        y = jugador->y;
        break;
    case 'A':
        x--;
        movimientoValido = ActualizarPosicion(laberinto, jugador, x, y);
        x = jugador->x;
        break;

    case 'S':
        y++;
        movimientoValido = ActualizarPosicion(laberinto, jugador, x, y);
        y = jugador->y;
        break;

    case 'D':
        x++;
        movimientoValido = ActualizarPosicion(laberinto, jugador, x, y);
        x = jugador->x;
        break;

    default:
        printf("Movimiento no valido\n");
        break;

    }
    }while(movimientoValido == PARED);

    if(movimientoValido == SALIDA)
    {
        return SALIDA;
    }
    return VALIDO;

}

void Ejecutar(Laberinto laberinto, Jugador* jugador) ///Funcion que ejecuta el juego hasta encontrar la salida
{
    int completado = VALIDO;

    while(1)
    {
        completado = PedirMovimiento(laberinto, jugador);

        if(completado == SALIDA)
        {
            break;
        }

        DibujarLaberinto(laberinto, jugador, 0);
    }

    printf("Has salido del laberinto!");
}
