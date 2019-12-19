#include "transforma.h"

struct _AA{
    Nh **estados;
    int indice_estados; /*Con esta variable indicamos el estado en que nos encontramos actualmente*/
    int num_estados;
};

Bool aa_esta_nh(AA *aa, Nh *nh);

/** 
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función que inicializa la estrutura global 
 */
AA *aa_ini(AFND * p_afnd){

    AA *aa = NULL;

    aa = (AA*)calloc(1, sizeof(AA));

    aa->estados = (Nh**)calloc(AFNDNumEstados(p_afnd)*2, sizeof(Nh*));

    return aa;
}

/** 
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función que libera la estructura global
 */
void aa_liberar(AA *aa){

    int i = 0;

    if(!aa) return;

    for(i = 0; i < aa->num_estados; i++){
        nh_liberar(aa->estados[i]);
    }
    free(aa->estados);

    free(aa);

    return;
}

/** 
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función que añade un nuevo estado a la estructura global
 */
Status aa_add_estado(AA *aa, Nh *nh){

    /*Con esta condición comprobamos que no se puedan añadir estados repetidos al array de estados*/
    if( aa_esta_nh(aa, nh) == TRUE) return ERROR;

    aa->estados[aa->num_estados] = nh;
    aa->num_estados++;


    return OK;
}

/** 
 *Autores: Andrea Salcedo y Alejandro Tejada
 * Función que comprueba si un nuevo estado ya esta en la estructura global
 */
Bool aa_esta_nh(AA *aa, Nh *nh){

    int i = 0;

    /*Recorremos el array de estados. Comprobamos que no se repta ningún estado en el array comparando sus índices*/
    for (i = 0; i < aa->num_estados; i++){
        if(cmp_nh(aa->estados[i], nh) == TRUE) return TRUE;
    }
    
    return FALSE;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función que imprime la estructura global
 */
void aa_print(AA *aa){

    int i = 0;

    printf("\n");
    printf("> Número de estados: %d\n", aa->num_estados);
    printf("> Índice: %d\n", aa->indice_estados);
    for(i = 0; i < aa->num_estados; i++){
        printf("%d: ", i);
        nh_print(aa->estados[i]);
    }

    printf("\n");
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función que encuentra los nuevos estados
 */
int find_news_nh(AA *aa, AFND * p_afnd){

    int inicio = 0, i = 0, j = 0, k = 0, afnd_numE = 0, afnd_numS = 0, *tabla = NULL, tamTabla = 0, x = 0, s = 0;
    Nh *nh = NULL;

    afnd_numE = AFNDNumEstados(p_afnd);/*Conseguimos el número de estados del autómata*/
    afnd_numS = AFNDNumSimbolos(p_afnd);/*Conseguimos el número de símbolos del autómata*/

    /*Solo va a entrar en esta condición cuando estemos en el estado inicial*/
    if(aa->num_estados == 0){ /*Si nos encontramos en el estado inicial, es decir todavía no se ha añadido ningún estado al array de estados*/
        inicio = AFNDIndiceEstadoInicial(p_afnd);
        nh = nh_ini(p_afnd);/*Creamos el estado para añadirlo después al array de estados*/
        nh_set_cEstados(nh, inicio); /*Añadimos el indice del estado inicial al nuevo estado*/
        for (i = 0; i < afnd_numE; i++){
            /*Aquí comprobamos si el estado inicial tiene alguna transición lambda con algún otro estado del autómata*/
            /*inicio != i, esto lo hacemos por si transita a si mismo con lambda ¿Eso puede darse?*/
            if(AFNDCierreLTransicionIJ(p_afnd, inicio, i) && inicio != i){
                /*Si transita con lambda, directamente lo añadimos al array de indices del nuevo estado*/
                nh_set_cEstados(nh, i); 
            }
        }
        /*Y directamente añadimos el nuevo estado al array de estados*/
        aa_add_estado(aa, nh);
        return 0;
    }
    

    tabla = nh_get_cEstados(aa->estados[aa->indice_estados]);/*Aqui conseguimos el array de indices de estados por los que esta formado el estado indicado por indice_estados*/
    tamTabla = nh_get_nEstados(aa->estados[aa->indice_estados]);/*Conseguimos el número de estados que forman el array*/

    printf("============> indice_estados: [%d] <===============\n", aa->indice_estados);

    printf("Imprimimos tabla INICIAL: ");
    nh_print(aa->estados[aa->indice_estados]);

    printf("Numero de estados de ese estado: ");
    printf("%d\n", tamTabla);

    printf("\n");
    
    /*Aqui lo que hacemos es comprobar cada uno de los estados de array de estados, 
    Comprobamos ha que estados transitan, y si alguno de ellos es lambda,
    Si ese estado ya esta añadido en el array de estados, no lo añade y liberamos el nh(nuevo estado) creado*/

    for (k = 0; k < afnd_numS; k++){
        nh = nh_ini(p_afnd); 
        for ( i = 0; i < tamTabla; i++){
            for(j = 0; j < afnd_numE; j++){
                /*Para el mismo símbolo, comprobamos las transiciones de la tabla de estados, con los estados del autómata original*/
                if(AFNDTransicionIndicesEstadoiSimboloEstadof(p_afnd, tabla[i], k, j)){
                    /*Si existe una transición de el estado de la tabla[i] al estado j con el símbolo k, lo añadimos al estado creado anteriormente*/
                    nh_set_cEstados(nh, j);
                    for(x = 0; x < afnd_numE; x++){
                        /*Entonces aquí comprobamos, si el indice del estado añadido al nh posee alguna transición lambda, si es así, tenemos que añadir ese índice también a nh*/
                        /*j != x, esto lo hacemos por si transita a si mismo con lambda ¿Eso puede darse?, ponerlo por si acaso, aunque en este ejemplo no pasa*/
                        if(AFNDCierreLTransicionIJ(p_afnd, j, x) && j != x){
                            nh_set_cEstados(nh, x); 
                        }
                    }
                }
            }
        }
        /*Cuando vamos a pasar al siguiente símbolo, añadimos el estado al array de estadosdel autómata determinista*/
        if(nh_get_nEstados(nh) != 0){/*Si no ha habido ninguna transición, es decir, el estado creado esta vacío y su número de estados es 0, simplemente eliminamos el nuevo estado*/
            printf("Imprimimos el nuevo estado: ");
            nh_print(nh);
            /*Si el nuevo estado no esta vacio, tiene indices, lo añadimos al array de estados del autómata*/
            /*Si al añadirlo, da un error porque dice que ya esta en el array, simplemente liberamos el nodo*/
            if(aa_add_estado(aa, nh) == ERROR) nh_liberar(nh);
        } 

        else nh_liberar(nh);
    }

    /*Una vez terminado todo, aumentamos el indice_estados para realizar el mismo procedimiento, con el siguiente estado del array de estados*/
    printf("\n");
    aa->indice_estados++;
    /*Si llegamos a ver que el número de estados del automata no determinista es igual al indice de estados, paramos el bucle y terminamos de construir el automata determinista*/
    if(aa->num_estados == aa->indice_estados) return 1;
    else return 0;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Funcion que obtiene el nombre de un nuevo estado
 */
char *nombre_nh(int *tabla, AFND *afnd, int tamTabla){

    int i = 0;
    char *nombrefinal = NULL, *nombre = NULL;

    nombrefinal = (char*)calloc(100, sizeof(char)); 

    for(i = 0; i < tamTabla; i++){
        nombre = (char*)calloc(strlen(AFNDNombreEstadoEn(afnd, tabla[i]))+1, sizeof(char));
        strcpy(nombre, AFNDNombreEstadoEn(afnd, tabla[i]));
        strcat(nombrefinal, nombre);
        free(nombre);
    }

    return nombrefinal;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Funcion que obtiene las transiciones de los nuevos estados
 */
Status obtener_transiciones(AA *aa, AFND *afnd, AFND *new_afnd){

    int i = 0, j = 0, x = 0, k =0, *tabla_estados = NULL, *tabla_estados2 = NULL, tamTabla = 0, afnd_numE = 0, afnd_numS = 0, z = 0;
    char * nombre_f1 = NULL, *nombre_f2 = NULL;
    Nh *nh = NULL;

    afnd_numE = AFNDNumEstados(afnd);/*Conseguimos el número de estados del autómata*/
    afnd_numS = AFNDNumSimbolos(afnd);/*Conseguimos el número de símbolos del autómata*/

    /*Volvemos a calcular las transiciones de los estados*/
    /*Esta parte es la misma que la de find_news_nh*/
    for (z = 0; z < aa->num_estados; z++){
        tabla_estados = nh_get_cEstados(aa->estados[z]);
        tamTabla = nh_get_nEstados(aa->estados[z]);

        printf("\n");
        printf("********->Imprimimos tabla estados en obtener_transiciones: ");
        nh_print(aa->estados[z]);

        printf("********->Numero de estados de ese estado: ");
        printf("%d\n", tamTabla);
        printf("\n");

        for (k = 0; k < afnd_numS; k++){
            nh = nh_ini(afnd);
            for ( i = 0; i < tamTabla; i++){
                for(j = 0; j < afnd_numE; j++){
                    if(AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, tabla_estados[i], k, j)){
                        nh_set_cEstados(nh, j);
                        for(x = 0; x < afnd_numE; x++){
                            if(AFNDCierreLTransicionIJ(afnd, j, x) && j != x){
                                nh_set_cEstados(nh, x); 
                            }
                        }
                    }
                }
            }
            if(aa_esta_nh(aa, nh) == TRUE){ /*Aqui comprobamos que el nuevo estado se encuentra en el array de estados, si está lo insertamos en el autómata*/
                nombre_f1 = nombre_nh(tabla_estados, afnd, tamTabla);
                nombre_f2 = nombre_nh(nh_get_cEstados(nh), afnd, nh_get_nEstados(nh));
                /*Aqui en vez de añadir el estado al array de estados, lo añadimos a al autómata*/
                AFNDInsertaTransicion(new_afnd, nombre_f1, AFNDSimboloEn(afnd, k), nombre_f2);
                free(nombre_f1);
                free(nombre_f2);
                nh_liberar(nh);
            } 
            else nh_liberar(nh);
        }
    }

    return OK;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función que declara el automata transformado, toda la información, los símbolos, los estados y las transiciones
 */
AFND *declarar(AA *aa, AFND *afnd){
    int i = 0, j = 0, *tabla = NULL, tamTabla = 0, flag = NORMAL;
    char *nombrefinal = NULL;
    AFND *new_afnd = NULL;

    new_afnd = AFNDNuevo("new_afd", aa->num_estados, AFNDNumSimbolos(afnd));

    for (i = 0; i < AFNDNumSimbolos(afnd); i++){
        AFNDInsertaSimbolo(new_afnd, AFNDSimboloEn(afnd, i));
    }

    for (i = 0; i < aa->num_estados; i++){
        tabla = nh_get_cEstados(aa->estados[i]);
        printf("---------------------->Estado que nos toca declarar: ");
        nh_print(aa->estados[i]);
        if((tamTabla = nh_get_nEstados(aa->estados[i])) > 1){
            /*Obtenemos el número de indices, es decir, de estados de ese estado del array y comprobamos los tipos de cada uno*/
            nombrefinal = nombre_nh(tabla, afnd, tamTabla);
            for (j = 0; j < tamTabla; j++){
                if(tabla[i] == 0){
                    flag = INICIAL;
                }
                else if(AFNDTipoEstadoEn(afnd, tabla[j]) == FINAL && flag != INICIAL_Y_FINAL){
                    flag = FINAL;
                }
                else if(AFNDTipoEstadoEn(afnd, tabla[j]) == INICIAL_Y_FINAL){
                    flag = INICIAL_Y_FINAL;
                }
            }
            AFNDInsertaEstado(new_afnd, nombrefinal, flag);
            free(nombrefinal);
        }
        else{
            printf("TABLA[0]: %d\n", tabla[0]);
            /*Ponemos tabla[0], porque como solo tiene un valor, es decir, un estado, no ponemos i y simplemente cogemos ese estado y ya*/
             AFNDInsertaEstado(new_afnd, AFNDNombreEstadoEn(afnd, tabla[0]), AFNDTipoEstadoEn(afnd, tabla[0]));
        }
        
        flag = NORMAL;
    }

    obtener_transiciones(aa, afnd, new_afnd);

    return new_afnd;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Función transforma que devuelve el autómata no determinista en determinista
 */
AFND * AFNDTransforma(AFND * afnd){

    AA *aa = NULL;
    int a = 0;
    AFND *afd = NULL;

    aa = aa_ini(afnd);
    
    while (!find_news_nh(aa, afnd));
    
    aa_print(aa);

    afd = declarar(aa, afnd);

    aa_liberar(aa);
    return afd;
}