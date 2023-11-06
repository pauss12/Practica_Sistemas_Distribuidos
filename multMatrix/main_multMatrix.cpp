#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "matrix_stub.h"

void freeMatrix(matrix_t* m){
    delete[] m->data;
    delete[] m;

}

int main()
{
    multMatrix_stub* mmatrix=new multMatrix_stub();
    matrix_t *m3 = mmatrix->leerMatriz("resultado.txt");
    /*
    matrix_t* m1= mmatrix->crearRandom(5,5);
    matrix_t* m2= mmatrix->crearIdentidad(5,5);
    matrix_t* mres=mmatrix->multiplicarMatrices(m1,m2);
    mmatrix->escribirMatriz(mres,"resultado.txt");
    matrix_t* mres2=mmatrix->multiplicarMatrices(m1,m3);
    mmatrix->escribirMatriz(mres2,"resultado2.txt");

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(mres);
    freeMatrix(mres2);
    */

    freeMatrix(m3);

    delete mmatrix;
}