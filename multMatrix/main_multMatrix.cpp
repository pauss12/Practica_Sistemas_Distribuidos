#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "matrix_stub.h"

void freeMatrix(matrix_t* m){
    delete[] m->data;
    delete[] m;

}
/*
int main()
{
    multMatrix_stub *mmatrix=new multMatrix_stub();

    matrix_t* m1= mmatrix->crearRandom(5,5);

    //lo de debajo lo he añadido yo para probar
    mmatrix->escribirMatriz(m1, "pruebaCrearRandom.txt");

    matrix_t* m2= mmatrix->crearIdentidad(5,5);
    matrix_t* mres=mmatrix->multiplicarMatrices(m1,m2);
    mmatrix->escribirMatriz(mres,"HOLA.txt");
    matrix_t* m3=mmatrix->leerMatriz("resultado.txt");
    matrix_t* mres2=mmatrix->multiplicarMatrices(m1,m3);
    mmatrix->escribirMatriz(mres2,"resultado2.txt");

    freeMatrix(m1);

    freeMatrix(m2);
    freeMatrix(mres);

    freeMatrix(m3);
    freeMatrix(mres2);

    delete mmatrix;
}
*/
// MAIN QUE NOS DAN AL PRINCIPIO, EL QUE DESCOMENTARE DESPUES DE LAS PRUEBAS

int main()
{
    multMatrix_stub *mmatrix = new multMatrix_stub();

    matrix_t *m1 = mmatrix->crearRandom(5, 5);
    matrix_t *m2 = mmatrix->crearIdentidad(5, 5);
    matrix_t *mres = mmatrix->multiplicarMatrices(m1, m2);
    mmatrix->escribirMatriz(mres, "resultado.txt");
    matrix_t *m3 = mmatrix->leerMatriz("resultado.txt");
    matrix_t *mres2 = mmatrix->multiplicarMatrices(m1, m3);
    mmatrix->escribirMatriz(mres2, "resultado2.txt");

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(mres);
    freeMatrix(m3);
    freeMatrix(mres2);
    delete mmatrix;
}
