#include <stdio.h>

typedef struct
{
    int pos[2];

} coord; //Estructura para definir las coordenadas del jugador dentro del laberinto

int main()
{
    int opc, dificultad;

    do
    {
        printf("MENU DEL LABERINTO\n\n");
        printf("1) Jugar\n");
        printf("2) Salir\n");

        scanf("%d", &opc);

        switch(opc)
        {

        case 1:
            do
            {

                printf("\nSeleccione el grado de dificultad:\n\n");
                printf("1) Principiante\n");
                printf("2) Veterano\n");
                printf("3) Experto\n");
                printf("4) Volver atras\n");

                scanf("%d", &dificultad);

                switch(dificultad)
                {
                case 1:
                    break;

                case 2:
                    break;

                case 3:
                    break;

                case 4:
                    break;

                default:
                    printf("Error, introduzca de nuevo la dificultad\n");
                    break;
                }

            } while(dificultad != 4);

            break;

        case 2:
            break;

        default:
            printf("Error, no existe opcion la introducida");

        }

    } while(opc != 2);


    return 0;
}
