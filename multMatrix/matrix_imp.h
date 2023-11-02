#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

class matrix_imp {
private: 
    int clientId = -1;
    multMatrix* m = nullptr;

public:
    matrix_imp(int clientId) : clientId(clientId) {}

    bool connectionClosed() {
        return p == nullptr;
    }

    void recibeOp() {
        std::vector<unsigned char> rpcIn;
        std::vector<unsigned char> rpcOut;
        // Recibe operación
        recvMSG(clientId, rpcIn);

        // Desempaqueta la operación
        personaOp operacion = unpack<personaOp>(rpcIn);

        // Realiza la operación correspondiente
        switch (operacion) {
        case ConstructorOp:
            if (m) {
                delete m;
                m = nullptr;
            }
            m = new multMatrix();
            pack(rpcOut, (unsigned char)MSG_OK);
            break;
        case DestructorOp:
            if (m) {
                delete m;
                m = nullptr;
                pack(rpcOut, (unsigned char)MSG_OK);
            } else {
                std::cout << "La instancia de multMatrix no está creada" << std::endl;
                pack(rpcOut, (unsigned char)MSG_NOK);
            }
            break;
        case ReadMatrixOp:
            if (m) {
                const char* fileName = /* Obtener el nombre del archivo de algún lugar */;
                matrix_t* result = m->readMatrix(fileName);
                if (result) {
                    // Realizar operaciones con la matriz leída
                    // Por ejemplo, puedes almacenar 'result' en una variable miembro de la clase
                    pack(rpcOut, (unsigned char)MSG_OK);
                } else {
                    std::cout << "Error al leer la matriz desde el archivo" << std::endl;
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
            } else {
                std::cout << "La instancia de multMatrix no está creada" << std::endl;
                pack(rpcOut, (unsigned char)MSG_NOK);
            }
            break;
        case MultMatricesOp:
            if (m) {
                // Obtener las matrices m1 y m2 desde algún lugar
                matrix_t* m1 = /* Obtener la primera matriz */;
                matrix_t* m2 = /* Obtener la segunda matriz */;

                matrix_t* result = m->multMatrices(m1, m2);
                if (result) {
                    // Realizar operaciones con la matriz resultante
                    // Por ejemplo, puedes almacenar 'result' en una variable miembro de la clase
                    pack(rpcOut, (unsigned char)MSG_OK);
                } else {
                    std::cout << "Error al multiplicar las matrices" << std::endl;
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
            } else {
                std::cout << "La instancia de multMatrix no está creada" << std::endl;
                pack(rpcOut, (unsigned char)MSG_NOK);
            }
            break;
        // Otras operaciones
        default:
            std::cout << "Error: función no definida" << std::endl;
            pack(rpcOut, (unsigned char)MSG_NOK);
            break;
        }

        // Envía resultados
        sendMSG(clientId, rpcOut);
    }
};
