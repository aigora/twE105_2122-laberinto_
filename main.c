#include <stdio.h>
#include <stdlib.h> //funcion malloc()
#include <ctype.h> //funcion toupper()
#include <math.h> //funcion sqrt()

#define LONGITUD_ARCHIVO_MAX 40 //El nombre del archivo del mapa, como mucho podrá ser de hasta 39 caracteres
#define SALIDA 1
#define VALIDO 2
#define PARED 0

typedef struct
{
    int x;
    int y;
}Coordenadas;

typedef struct
{
    int largo;
    int ancho;
    int dimensiones;
    int visibilidad;
    char* casillas;
    Coordenadas* salidas; //Ubicacion de las salidas en el laberinto
    int nSalidas; //Numero de salidas del laberinto
}Laberinto;

typedef struct
{
    float x;
    float y;
}CoordenadasReales;

enum Estados
{
    eInicio = 0,
    eJuego,
    eCreditos,
    eTutorial,
    eSalir
};

void DibujarLaberinto(Laberinto laberinto, Coordenadas* jugador, int indices);
int LeerArchivo(const char* archivo, Laberinto* laberinto);
int RellenarCasillas(const char* archivo, Laberinto* laberinto);
void PedirArchivo(Laberinto* laberinto);
int ActualizarPosicion(Laberinto laberinto, Coordenadas* jugador, int x, int y);
int PedirMovimiento(Laberinto laberinto, Coordenadas* jugador);
void Ejecutar(Laberinto laberinto, Coordenadas* jugador);
void PedirUbicacionJugador(Laberinto laberinto, Coordenadas* jugador);
int Restringir(int x, int min, int max);
void Jugar();
void Creditos();
void Tutorial();
void ActualizarEstado(int opcion, enum Estados* estado);
void DarPista(int x, int y, Coordenadas* salidas, int nsalidas);
CoordenadasReales Normalizar(Coordenadas coords);

int main()
{
    enum Estados estado = eInicio;

    int opc;


    printf("------------MENU LABERINTO----------\n");
    printf("(1) Jugar\n(2) Creditos\n(3) Tutorial\n(4) Salir\n");
    scanf("%d", &opc);

   ActualizarEstado(opc, &estado);

    while(estado != eSalir)
    {
        if(estado == eInicio)
        {
            printf("------------MENU LABERINTO----------\n");
            printf("(1) Jugar\n(2) Creditos\n(3) Tutorial\n(4) Salir\n");
            scanf("%d", &opc);
        }
        else if(estado == eJuego)
        {
            Jugar();
        }
        else if(estado == eCreditos)
        {
            Creditos();
            scanf("%d", &opc);
        }
        else if(estado == eTutorial)
        {
            Tutorial();
            scanf("%d", &opc);
        }

        ActualizarEstado(opc, &estado);

    }

    return 0;
}

void PedirUbicacionJugador(Laberinto laberinto, Coordenadas* jugador) ///Pide al usuario las coordenadas en las quiere aparecer al comienzo del juego
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
            //Se guardan las coords iniciales en la estructura Jugador
            posicionCorrecta = 1;
            jugador->x = x;
            jugador->y = y;
        }
    }
    while(posicionCorrecta != 1);
}

int RellenarCasillas(const char* archivo, Laberinto* laberinto) ///Rellena las casillas del laberinto a partir de los valores leidos y guarda la informacion de las salidas
{
    int contador = 0;
    laberinto->nSalidas = 0;
    int posSalida = 0; //Contador de posicion de salidas rellenadas
    int i, j;
    FILE* pf = fopen(archivo, "r"); //Abre el archivo introducido por teclado

    if(pf != NULL)
    {
        char valor;

        while(fscanf(pf, "%c", &valor) != EOF) //Lee caracter a caracter el archivo
        {
            if(valor != '\n' && valor != ' ') //Si no es un salto de línea o un espacio, entonces es un caracter
            {
                laberinto->casillas[contador] = valor; //Almacena el valor leido del archivo en la casilla correspondiente del laberinto
                contador++;
            }

        }

        //Las salidas solo se encontraran en los bordes
        for(i = 0; i < laberinto->largo; i++) //Recorre el borde lateral izqdo
        {
            int indice = laberinto->ancho * i + 0;
            if(laberinto->casillas[indice] == 'G')
                laberinto->nSalidas++;
        }

        for(j = 0; j < laberinto->ancho; j++) //Recorre el borde superior
        {
            int indice = laberinto->ancho * 0 + j;
            if(laberinto->casillas[indice] == 'G')
                laberinto->nSalidas++;
        }

        for(i = 0; i < laberinto->largo; i++) //Recorre el borde lateral dcho
        {
            int indice = laberinto->ancho * i + (laberinto->ancho - 1);
            if(laberinto->casillas[indice] == 'G')
                laberinto->nSalidas++;
        }

        for(j = 0; j < laberinto->ancho; j++) //Recorre el borde inferior
        {
            int indice = laberinto->ancho * (laberinto->largo - 1) + j;
            if(laberinto->casillas[indice] == 'G')
                laberinto->nSalidas++;
        }

        laberinto->salidas = (Coordenadas*)malloc(sizeof(Coordenadas)*laberinto->nSalidas);

        for(i = 0; i < laberinto->largo; i++)
        {
            int indice = laberinto->ancho * i + 0;
            if(laberinto->casillas[indice] == 'G')
            {
                laberinto->salidas[posSalida].x = 0;
                laberinto->salidas[posSalida].y = i;
                posSalida++;
            }
        }

        for(j = 0; j < laberinto->ancho; j++) //Recorre el borde superior
        {
            int indice = laberinto->ancho * 0 + j;
            if(laberinto->casillas[indice] == 'G')
            {
                laberinto->salidas[posSalida].x = j;
                laberinto->salidas[posSalida].y = 0;
                posSalida++;
            }
        }

        for(i = 0; i < laberinto->largo; i++) //Recorre el borde lateral dcho
        {
            int indice = laberinto->ancho * i + (laberinto->ancho - 1);
            if(laberinto->casillas[indice] == 'G')
            {
                laberinto->salidas[posSalida].x = laberinto->ancho - 1;
                laberinto->salidas[posSalida].y = i;
                posSalida++;
            }
        }

        for(j = 0; j < laberinto->ancho; j++) //Recorre el borde inferior
        {
            int indice = laberinto->ancho * (laberinto->largo - 1) + j;
            if(laberinto->casillas[indice] == 'G')
            {
                laberinto->salidas[posSalida].x = j;
                laberinto->salidas[posSalida].y = laberinto->largo - 1;
                posSalida++;
            }
        }

        free(laberinto->salidas);
        fclose(pf);
        return 1; //Lectura correcta
    }


    return 0; //Error al abrir el archivo
}

int LeerArchivo(const char* archivo, Laberinto* laberinto) ///Lee el mapa del laberinto contenido en el documento de texto
{
    int primeraLinea = 0; //Variable auxiliar para contar el numero de columnas, solo se contaran en la primera linea leida del fichero
    int filas = 1;
    int columnas = 1;

    FILE* pf = NULL;
    pf = fopen(archivo, "r");

    if(pf != NULL)
    {
        char valor;

        while(fscanf(pf, "%c", &valor) != EOF)
        {
            if(valor == '\n') //Cada salto de linea, incrementa el numero de filas
            {
                filas++;
                primeraLinea = 1;
            }

            else if(valor == ' ' && primeraLinea == 0) //Cada espacio, incrementa el numero de columnas
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

void PedirArchivo(Laberinto* laberinto) ///Pide al usuario que introduzca el mapa por teclado
{
    int resultadoLectura = 0;

    char archivo[LONGITUD_ARCHIVO_MAX];

    printf("Introduzca el archivo del laberinto (mapa_1.txt,mapa_2.txt): ");


    do //Pide el archivo hasta que se introduzca uno valido
    {
        scanf("%s", archivo); //El nombre del archivo introducido por teclado debe ser <nombrearchivo>.<txt>
        resultadoLectura = LeerArchivo(archivo, laberinto); //LeerArchivo devuelve 0 en caso de no poder leer el archivo

        if(resultadoLectura == 0) /+
        {
            printf("El archivo no es valido, introduzca uno valido: ");
        }
    }while(resultadoLectura != 1);
}

int Restringir(int x, int min, int max) ///Limita la visibilidad del jugador
{
    if(x < min)
        return min;

    if(x > max)
        return max;

    return x;

    //La funcion "restringe" un intervalo de datos comprendido entre min y max, si el dato x se encuentra en el intervalo devolvera x
    //Pero si sale del intervalo por debajo, devolvera min y si sale por arriba devolvera el max

}

void DibujarLaberinto(Laberinto laberinto, Coordenadas* jugador, int indices) ///Imprime por pantalla el laberinto
{
    int i = 0;
    int j = 0;
    int posicionJugador;

    int minX = 0, maxX = laberinto.ancho;
    int minY = 0, maxY = laberinto.largo;


    if(jugador != NULL)
    {
        posicionJugador = laberinto.ancho * jugador->y + jugador->x; //Convierte las coords 2D a 1D del jugador, "vectoriza" a la "matriz jugador"

        if(indices == 0) //Solo limitara la vision cuando indices este desactivado, despues de introducir las coordenadas iniciales del jugador
        {
            minX = Restringir(jugador->x - laberinto.visibilidad, 0, laberinto.ancho);
            maxX = Restringir(jugador->x + laberinto.visibilidad, 0, laberinto.ancho);
            minY = Restringir(jugador->y - laberinto.visibilidad, 0, laberinto.largo);
            maxY = Restringir(jugador->y + laberinto.visibilidad, 0, laberinto.largo);
        }
    }

    if(indices == 1) //Si indices es 1 (TRUE) entonces enumera las filas y columnas del laberinto
    {


        printf("     ");
        for(i = 0; i < laberinto.ancho; i++)
        {
            if(i < 10)
                printf("%d  ", i);

            else
                printf("%d ", i);
        }

        printf("\n");
    }

    for(i = minY; i < maxY; i++) //Bucles anidados para recorrer el laberinto
    {
        if(indices == 1)
        {
            if(i < 10)
                printf("%d    ", i);
            else if(i >= 10 && i <= 99)
                printf("%d   ", i);
            else
                printf("%d  ", i);
        }

        for(j = minX; j < maxX; j++)
        {
            int indice = laberinto.ancho * i + j; //Vectoriza la matriz que contiene al laberinto

            if(jugador != NULL && indice == posicionJugador) //Si las coordenadas 1D de la matriz y el jugador coinciden y no contienen una pared, entonces imprime al jugador
            {
                printf("X  "); //La X simboliza al jugador
            }
            else
            {
                printf("%c  ", laberinto.casillas[indice]); //Mostrar casilla
            }
        }
        //Siguiente fila
        printf("\n");
    }

    printf("\n");
}

int ActualizarPosicion(Laberinto laberinto, Coordenadas* jugador, int x, int y) ///Movimiento del jugador
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
        if(laberinto.casillas[indice] == '!')
        {
            printf("\nPISTA!!!\n");
            DarPista(x, y, laberinto.salidas, laberinto.nSalidas);
            printf("\n");
        }
        return VALIDO;
    }

}

int PedirMovimiento(Laberinto laberinto, Coordenadas* jugador) ///Pide al usuario que introduzca un movimiento
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

void Ejecutar(Laberinto laberinto, Coordenadas* jugador) ///Ejecuta el juego hasta encontrar la salida (gameloop)
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

    printf("Has salido del laberinto!\n");
    system("PAUSE");
    exit(1);
}

void Jugar()
{
    Laberinto laberinto;
    Coordenadas jugador;

    PedirArchivo(&laberinto);

    DibujarLaberinto(laberinto, NULL, 1);
    /*Imprime el laberinto por primera vez
    Como aun no se han introducido las coordenadas iniciales el argumento "jugador" será NULL
    Y el argumento "indices" será 1, para enumerar filas y columnas y facilitar al usuario la colocacion del jugador*/

    PedirUbicacionJugador(laberinto, &jugador);

    printf("Introduzca la visibilidad: ");
    scanf("%d", &laberinto.visibilidad);

    DibujarLaberinto(laberinto, &jugador, 0);
    /*Tras introducir las coordenadas iniciales y la visibilidad el juego comienza y se desactivan
    los indices para facilitar la lectura del usuario*/

    Ejecutar(laberinto, &jugador);

    free(laberinto.casillas);
}

void Creditos()
{
    printf("CREDITOS\n");
    printf("Integrantes del grupo:\n\n");
    printf("Diego Fernandes Alves (55229).\n");
    printf("Mario Carrion Sirvent (55174).\n");
    printf("Dionisio Caballero Garcia (55541).\n");
    printf("Osama Boutman El-Khattabi (55151).\n");
    printf("\n\n");
    printf("Pusla 0 para salir!\n");
}

void Tutorial()
{
    printf("TUTORIAL\n");
    printf("Pusla 0 para salir!\n");
}

void ActualizarEstado(int opcion, enum Estados* estado) ///Permite el estado de cada opcion del menu, al moverse por el menu
{
    if(*estado == eInicio)
    {
        switch (opcion)
        {
        case 1:
            *estado = eJuego;
            break;
        case 2:
            *estado = eCreditos;
            break;
        case 3:
            *estado = eTutorial;
            break;
        case 4:
            *estado = eSalir;
            break;
        default:
            break;
        }
    }
    /*Si el usuario esta en el menu de inicio, y pulsa cualquier opcion el programa guarda
    la opcion pulsada en el estado correspondiente */

    else if(*estado == eCreditos || *estado == eTutorial)
    {
        switch (opcion)
        {
        case 0:
            *estado = eInicio;
            break;
        default:
            break;
        }
    }

    /*Si el usario se encuentra en creditos o en tutorial, al pulsar 0
    volverá al estado eInicio correspondiente al menu de inicio*/
}
CoordenadasReales Normalizar(Coordenadas coords) ///Normaliza un vector de 2D
{
    float modulo = sqrt(coords.x*coords.x + coords.y*coords.y);
    CoordenadasReales coordsNorm;

    coordsNorm.x /= modulo;
    coordsNorm.y /= modulo;

    return coordsNorm;
}

void DarPista(int x, int y, Coordenadas* salidas, int nsalidas)
{
    int i;
    int xmin = 0, ymin = 0;
    float distmin = 9999; //Se elige una distancia min lo suficientemente grande para hacer la primera comparacion
    float dist;

    /*Para ver a que distancia se encuentra el jugadro de una salida se restan las coordenadas del jugador
    con las coordenadas de dicha salida, dado que al restarse, se obtiene un vector resultante que apunta desde
    la posicion del jugador hacia la salida

    /*Para saber cual es la salida mas cercana es necesario obtener estos vectores resultantes, normalizarlos hallar
    su valor absoluto y compararlos con su valor con signo.

    Si p.e el vector normalizado en valor absoluto es (0.84, 0.2) entonces el jugador tendra que moverse muy poco
    verticalmente y bastante longitudinalmente para saber si es hacia arriba o abajo, o izquierda derecha, se compara
    con su valor con signo, si fuera (-0.84, 0.2) entonces el jugador se tendria que desplazar muy poco hacia abajo y mucho
    hacia la derecha*/

    for(i=0; i<nsalidas; i++)
    {
        Coordenadas c = salidas[i]; //Guardamos la salida actual en c
        Coordenadas vector;
        vector.x = c.x - x;
        vector.y = c.y - y;

        dist = (vector.x * vector.x) + (vector.y * vector.y);

        if(dist<distmin)
        {
            distmin = dist;
            xmin = vector.x;
            ymin = vector.y;
            //Vector hacia la salida mas cercana
        }
    }

    Coordenadas min;
    min.x = xmin;
    min.y = ymin;

    CoordenadasReales normalizado = Normalizar(min);

    CoordenadasReales normalizadoAbs = normalizado; //No queremos perder las coordenadas normalizadas originales para luego comparar
    normalizadoAbs.x = fabs(normalizadoAbs.x)*100.0f;
    normalizadoAbs.y = fabs(normalizadoAbs.y)*100.0f;


    //Se establecen unos valores porcentuales, para que el programa interprete que es mucho o poco
    if(normalizadoAbs.x >= 0.0f && normalizadoAbs.x <= 25.0f)
    {
        printf("Muevete MUY POCO a la ");
    }
    else if(normalizadoAbs.x >= 25.0f && normalizadoAbs.x <= 50.0f)
    {
        printf("Muevete un POCO a la ");
    }
    else if(normalizadoAbs.x >= 50.0f && normalizadoAbs.x <= 75.0f)
    {
        printf("Muevete BASTANTE a la ");
    }
    else if(normalizadoAbs.x >= 75.0f && normalizadoAbs.x <= 100.0f)
    {
        printf("Muevete MUCHISIMO a la ");
    }

    if(normalizado.x < 0)
        printf("Izquierda");
    else
        printf("Derecha");

    printf(" y ");

    if(normalizadoAbs.y >= 0.0f && normalizadoAbs.y <= 25.0f)
    {
        printf("MUY POCO hacia ");
    }
    else if(normalizadoAbs.y >= 25.0f && normalizadoAbs.y <= 50.0f)
    {
        printf("un POCO hacia ");
    }
    else if(normalizadoAbs.y >= 50.0f && normalizadoAbs.y <= 75.0f)
    {
        printf("BASTANTE hacia ");
    }
    else if(normalizadoAbs.y >= 75.0f && normalizadoAbs.y <= 100.0f)
    {
        printf("MUCHISIMO hacia ");
    }

    if(normalizado.x < 0)
        printf("Arriba\n");
    else
        printf("Abajo\n");
}
