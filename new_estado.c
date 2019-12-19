#include "new_estado.h"

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Estructura de las clases del Nh cociente
 */
struct _Nh{
    int * cEstados;
    int numEstados;
}; 

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que reserva memoria suficiente para una clase
 */
Nh * nh_ini(AFND * p_afnd){
    
    Nh * c = NULL;
    int i = 0;

    c = (Nh*)calloc(1, sizeof(Nh));
    if(!c){
        fprintf(stderr, "Error a a hora de reservar memoria para un Nh\n");
        return NULL;
    }

    /*Reservamos memoria para un Nh de estados que siempre será el numero de estados del automata */
    c->cEstados = (int*)calloc(AFNDNumEstados(p_afnd), sizeof(int));
    if(!c->cEstados){
        fprintf(stderr, "Error a a hora de reservar memoria para los estados de un Nh\n");
        return NULL;
    }

    for(i = 0; i < AFNDNumEstados(p_afnd); i++){
        c->cEstados[i] = -1;
    }

    c->numEstados = 0;

    return c;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que libera la memoria reservada para una clase
 */
void nh_liberar(Nh *c){

    if(!c) return;

    if(c->cEstados) free(c->cEstados);

    free(c);

    return;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que añade un indice de un estado del automata a la clase
 */
int nh_set_cEstados(Nh * c, int estados){

    if(!c || estados < 0){
        fprintf(stderr, "Error, los parametros Nh y estados son nulos\n");
        return -1;
    }

    if(esta_en_nh(c, estados) == TRUE) return -1;

    c->cEstados[c->numEstados] = estados;

    c->numEstados++;

    return 0;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que modifica el numero de estados de la clase
 */
Nh * nh_set_nEstados(Nh * c, int numEstados){

    if(!c || numEstados < 0){
        fprintf(stderr, "Error, los parametros Nh y numEstados son nulos\n");
        return NULL;
    }

    c->numEstados = numEstados;

    return c;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que obtiene el número de indices que tiene la clase
 */
int nh_get_nEstados(Nh * c){

    if(!c){
        fprintf(stderr, "Error, el parametro Nh es nulo\n");
        return -1;
    }

    return c->numEstados;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que obtiene los indices de los estados que estan en la clase
 */
int * nh_get_cEstados(Nh * c){

    if(!c){
        fprintf(stderr, "Error, el parametro Nh es nulo\n");
        return NULL;
    }

    return c->cEstados;
}

/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que comprueba si un indice ya esta en la clase
 */
Bool esta_en_nh(Nh *c, int indice){

    int i = 0;

    for ( i = 0; i < c->numEstados; i++){
        if(c->cEstados[i] == indice) return TRUE;
    }

    return FALSE;  
}
/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que comprueba si dos nuevos estados son iguales
 */ 
Bool cmp_nh(Nh *nh1, Nh *nh2){

    int i = 0;

    /*printf("Estamos en la función cmp_nh, comparando los estados:\n");

    printf("ESTADO1: ");
    nh_print(nh1);

    printf("ESTADO2: ");
    nh_print(nh2);*/

    if(nh1->numEstados != nh2->numEstados) return FALSE;

    for(i = 0; i < nh1->numEstados; i++){
        if(esta_en_nh(nh2, nh1->cEstados[i]) == FALSE) return FALSE;
    }

    return TRUE;
}
/**
 * Autores: Andrea Salcedo y Alejandro Tejada
 * Metodo que imprime la clase
 */
int nh_print(Nh *c){

    int i = 0;

    if(!c) return -1;

    for(i = 0; i < c->numEstados ; i++){
        printf("%d ", c->cEstados[i]);
    }
    printf("\n");

    return 0;
}







