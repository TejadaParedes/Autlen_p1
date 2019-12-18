#include "transforma.h"

struct _AA{
    Nh **estados;
    int indice_estados;
    int num_estados;
};
Bool aa_esta_nh(AA *aa, Nh *nh);
/** 
 * Alex
 * Función que inicializa la estrutura global 
 */
AA *aa_ini(AFND * p_afnd){

    AA *aa = NULL;

    aa = (AA*)calloc(1, sizeof(AA));

    aa->estados = (Nh**)calloc(AFNDNumEstados(p_afnd)*2, sizeof(Nh*));

    return aa;
}

/** 
 * Alex
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
 * Alex
 * Función que añade un nuevo estado al la estructura global
 */
Status aa_add_estado(AA *aa, Nh *nh){

    if( aa_esta_nh(aa, nh) == TRUE) return ERROR;

    aa->estados[aa->num_estados] = nh;
    aa->num_estados++;


    return OK;
}

/** 
 * Alex
 * Función que comprueba si un nuevo estado ya esta en la estructura global
 */
Bool aa_esta_nh(AA *aa, Nh *nh){

    int i = 0;

    for (i = 0; i < aa->num_estados; i++){
        if(cmp_nh(aa->estados[i], nh) == TRUE) return TRUE;
    }
    
    return FALSE;
}

/**
 * Alex
 * Función que imprime la estructura global
 */
void aa_print(AA *aa){

    int i = 0;

    printf("> Número de estados: %d\n", aa->num_estados);
    printf("> Índice: %d\n", aa->indice_estados);
    for(i = 0; i < aa->num_estados; i++){
        printf("%d: ", i);
        nh_print(aa->estados[i]);
    }
}

/**
 * Alex
 * Función que encuentra los nuevos estados
 */
int find_news_nh(AA *aa, AFND * p_afnd){

    int inicio = 0, i = 0, j = 0, k = 0, afnd_numE = 0, afnd_numS = 0, *tabla = NULL, tamTabla = 0, x = 0, s = 0;
    Nh *nh = NULL;

    afnd_numE = AFNDNumEstados(p_afnd);
    afnd_numS = AFNDNumSimbolos(p_afnd);

    if(!aa->num_estados){
        inicio = AFNDIndiceEstadoInicial(p_afnd);
        nh = nh_ini(p_afnd);
        nh_set_cEstados(nh, inicio);
        for (i = 0; i < afnd_numE; i++){
            if(AFNDCierreLTransicionIJ(p_afnd, inicio, i) && inicio != i){
                /*Necesita a lo mejor una pensada mas*/
                /* for (j = i; j > 0; j--){*/
                nh_set_cEstados(nh, i); 
                /*}*/
                /*-------------------- */
            }
        }
        aa_add_estado(aa, nh);
        /*cambiar 
        if(!nh){
            nh = nh_ini(p_afnd);
            nh_set_cEstados(nh, inicio);
            aa_add_estado(aa, nh);
        }*/
        return 0;
    }
    /*sacar las transiciones del estado donde se encuentra el indice y añadir los nuevos esatdos a los que se transitan, y aumentar el indice, comprobar al final si el indice es igual al numero de nuevos estados*/
    
    tabla = nh_get_cEstados(aa->estados[aa->indice_estados]);
    tamTabla = nh_get_nEstados(aa->estados[aa->indice_estados]);
    /*transiciones lambdas (darle una pensada mas)*/
    /*for ( i = 0; i < tamTabla; i++){
        nh = nh_ini(p_afnd);
        nh_set_cEstados(nh, i);
        for(j = 0; j < afnd_numE; j++){
            if(AFNDCierreLTransicionIJ(p_afnd, inicio, i)){
                for (j = i; j > 0; j--){
                    nh_set_cEstados(nh, j);
                }
                aa_add_estado(aa, nh);
            }
        }
    }*/
    for (k = 0; k < afnd_numS; k++){
        nh = nh_ini(p_afnd);
        for ( i = 0; i < tamTabla; i++){
            for(j = 0; j < afnd_numE; j++){
                if(AFNDTransicionIndicesEstadoiSimboloEstadof(p_afnd, tabla[i], k, j)){
                    nh_set_cEstados(nh, j);
                    for(x = 0; x < afnd_numE; x++){
                        if(AFNDCierreLTransicionIJ(p_afnd, j, x) && j != x){
                            /*for (s = x; s > j; s--){*/
                                nh_set_cEstados(nh, x); /*Necesita a lo mejor una pensada mas*/
                            /* }*/
                        }
                    }
                }
            }
        }
        if(nh_get_nEstados(nh)) aa_add_estado(aa, nh);
        else nh_liberar(nh);
    }
    aa->indice_estados++;
    if(aa->num_estados == aa->indice_estados) return 1;
    else return 0;
}
/**
 * 
 * Funcion que obtiene las transiciones de los nuevos estados
 */
Status obtener_transiciones(AA *aa, AFND *afnd, AFND *new_afnd);
/**
 * Alex
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
 * Alex
 * Función que declara el automata transformado
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
        if((tamTabla = nh_get_nEstados(aa->estados[i])) > 1){
            nombrefinal = nombre_nh(tabla, afnd, tamTabla);
            for (j = 0; j < tamTabla; j++){
                if(/*AFNDTipoEstadoEn(afnd, tabla[j]) == INICIAL*/i == 0){
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
        }
        else{
             AFNDInsertaEstado(new_afnd, AFNDNombreEstadoEn(afnd, tabla[0]), AFNDTipoEstadoEn(afnd, tabla[0]));
        }
        free(nombrefinal);
        flag = NORMAL;
    }
    /*obetener las transiciones */
    return new_afnd;
}

AFND * AFNDTransforma(AFND * afnd){

    AA *aa = NULL;
    int a = 0;
    AFND *afd = NULL;

    aa = aa_ini(afnd);
    
    /* printf("#########\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("=========\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("=========\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("=========\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("=========\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("=========\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("=========\n");
    a = find_news_nh(aa, afnd);
    printf("Return: %d\n", a);
    printf("#########\n");*/

    while (!find_news_nh(aa, afnd));
    
    aa_print(aa);

    afd = declarar(aa, afnd);

    aa_liberar(aa);
    return afd;
}