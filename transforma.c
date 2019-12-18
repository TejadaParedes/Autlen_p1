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

    for(i = 0; i < aa->num_estados; i++){
        nh_liberar(aa->estados[i]);
    }

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
        nh_print(aa->estados[i]);
    }
}

/**
 * Alex
 * Función que encuentra los nuevos estados
 */
int find_news_nh(AA *aa, AFND * p_afnd){

    int inicio = 0, i = 0, j = 0, k = 0, afnd_numE = 0, afnd_numS = 0, *tabla = NULL, tamTabla = 0, flag = 0;
    Nh *nh = NULL;

    afnd_numE = AFNDNumEstados(p_afnd);
    afnd_numS = AFNDNumSimbolos(p_afnd);

    if(!aa->num_estados){
        inicio = AFNDIndiceEstadoInicial(p_afnd);
        for (i = 0; i < afnd_numE; i++){
            if(AFNDCierreLTransicionIJ(p_afnd, inicio, i) && inicio != i){
                nh = nh_ini(p_afnd);
                nh_set_cEstados(nh, inicio);
                for (j = i; j > 0; j--){
                    nh_set_cEstados(nh, j); /*Necesita a lo mejor una pensada mas*/
                }

                aa_add_estado(aa, nh);
            }
        }
        if(!nh){
            nh = nh_ini(p_afnd);
            nh_set_cEstados(nh, inicio);
            aa_add_estado(aa, nh);
        }
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

AFND * AFNDTransforma(AFND * afnd){

    AA *aa = NULL;

    aa = aa_ini(afnd);
    
    printf("#########\n");
    find_news_nh(aa, afnd);
    printf("=========\n");
    find_news_nh(aa, afnd);
    printf("=========\n");
    find_news_nh(aa, afnd);
    printf("=========\n");
    find_news_nh(aa, afnd);
    printf("#########\n");
    /* Falla porque falta la 5 en el 2 y 3 porque del 3 al 5 hay un lambda*/
    aa_print(aa);

    return NULL;
}