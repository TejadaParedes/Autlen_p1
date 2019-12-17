#ifndef NEW_ESTADO
#define NEW_ESTADO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afnd.h"
#include "types.h"

typedef struct _Nh Nh;


Nh * nh_ini(AFND * p_afnd);
void nh_liberar(Nh *c);
int  nh_set_cEstados(Nh * c, int estados);
Nh * nh_set_nEstados(Nh * c, int numEstados);
int nh_get_nEstados(Nh * c);
int * nh_get_cEstados(Nh * c);
Bool esta_en_nh(Nh *c, int indice);
Bool cmp_nh(Nh *nh1, Nh *nh2);
int nh_print(Nh *c);

#endif