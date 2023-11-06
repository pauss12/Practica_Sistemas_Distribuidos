#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

//MANDAR UNA MATRIZ DESDE EL SERVER--------------------------------------------------------------------------------------
void send_matriz_server(int clientId, operacionesEnum op, matrix_t matrix) {
    
	std::vector<unsigned char> rpcOut;
	
    // Empaquetar la operación
    pack(rpcOut, op);

    // Empaquetar el número de filas y columnas
    pack(rpcOut, matrix.rows);
    pack(rpcOut, matrix.cols);

    // Empaquetar los datos de la matriz
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            pack(rpcOut, matrix.data[i * matrix.cols + j]);
        }
    }

    // Envía la operación y la matriz al servidor
    sendMSG(clientId, rpcOut);
}
/*
//RECIBIR UNA MATRIZ EN EL SERVER--------------------------------------------------------------------------------------
multMatrix_stub recvMatrixOp(int serverId, const std::string& rutaArchivo, operacionesEnum op) {
    
	std::vector<unsigned char> rpcIn;
    std::vector<unsigned char> rpcOut;
	
	//Matriz que vamos a devolver
	multMatrix_stub matriz;
    
    // Empaqueta la operación
    pack(rpcOut, op);
    
    if (op == leerMatrizOp) {
        pack(rpcOut, static_cast<int>(rutaArchivo.size()));
        packv(rpcOut, rutaArchivo.data(), rutaArchivo.size());
    }

    // Envía la operación y otros parámetros al servidor
    sendMSG(serverId, rpcOut);

    // Recibe la respuesta del servidor
    recvMSG(serverId, rpcIn);

    // Desempaqueta la matriz
    int rows, cols;
	
    rows = unpack<int>(rpcIn);
    cols = unpack<int>(rpcIn);
	
    matriz.rows = rows;
    matriz.cols = cols;
    matriz.data = new int[rows * cols];
	
    for (int i = 0; i < rows * cols; i++) {
        matriz.data[i] = unpack<int>(rpcIn);
    }
	
	return matriz;
}

*/
//CLASE IMP (SERVER) --------------------------------------------------------------------------------------------------
class matrix_imp{

	private: 
		int clientId = -1;
		multMatrix *matriz_server = nullptr;

	public:
	
		matrix_imp(int clientId) : clientId(clientId) {};

		bool connectionClosed() {
			return matriz_server == nullptr;
		}

		void recibeOp() {
			
			std::vector<unsigned char> rpcIn;
			std::vector<unsigned char> rpcOut;
			
			//Recibe la operacion
			recvMSG(clientId, rpcIn);

			operacionesEnum operacion = unpack<operacionesEnum>(rpcIn);

			switch (operacion) {
				
				case opConstructor:
				{
					matriz_server = new multMatrix();
					pack(rpcOut, (unsigned char)MSG_OK);
					
				}break;
					
				case opDestructor:
				{
					if (matriz_server) {
						
						delete matriz_server;
						pack(rpcOut, (unsigned char)MSG_OK);
						
					} else {
						std::cout << "La instancia de multMatrix no está creada" << "\n"<< std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
					
				}break;
	
				case opLeerMatriz:
				{
					if (matriz_server)
					{
						//Creo la variable para tener la ruta del archivo (guardarlo en un char)
						std::string dato;

						//Recibo la cadena con su tamaño y contenido
						recv_cadena(clientId, dato);

						std::cout << "mensaje recibido del cliente" << std::endl;

						//Pasar la cadena a char
						const char* fileName = dato.c_str();

						//Llamar a la funcion para que lea la matriz en el server
						matrix_t *mmatriz = matriz_server->readMatrix(fileName);
						
						//Enviar la matriz
						send_matriz_server(clientId, operacion, *mmatriz);

						std::cout << "matriz enviada desde el server" << std::endl;

						//Recibir el ok
						recvMSG(clientId, rpcIn);
				
						if (rpcIn[0] != MSG_OK)
							std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";
						
					}else{
						
						std::cout << "La instancia de multMatrix no está creada" << "\n"<< std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
						
					}
				}break;
				/*
				case opCrearRandom:
				{
					int rows = unpack<int>(rpcIn);
					int cols = unpack<int>(rpcIn);

					if (rows > 0 && cols > 0)
					{
						matrix_t *randomMatrix = p->createRandMatrix(rows, cols);

						if (randomMatrix)
						{
							packMatrix(rpcOut, randomMatrix);
							pack(rpcOut, (unsigned char)MSG_OK);

							// Libera la memoria de la matriz aleatoria
							freeMatrix(randomMatrix);
						}
						else
						{
							pack(rpcOut, (unsigned char)MSG_NOK);
						}
					}
					else
					{
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
				}
				break;

				case opMultiplicarMatrices:
				{
					if (m) {
						matrix_t* result = m->multMatrices(m1, m2);

						if (result) {
							// Realizar operaciones con la matriz resultante
							// Por ejemplo, puedes almacenar 'result' en una variable miembro de la clase
							pack(rpcOut, (unsigned char)MSG_OK);
						}else {

							std::cout << "Error al multiplicar las matrices" << std::endl;
							pack(rpcOut, (unsigned char)MSG_NOK);
						}
					} else {
						std::cout << "La instancia de multMatrix no está creada" << std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
					}

				}break;

				case opCrearIdentidad:
				{
					int rows = unpack<int>(rpcIn);
					int cols = unpack<int>(rpcIn);

					if (rows > 0 && cols > 0) {
						matrix_t* identityMatrix = p->createIdentity(rows, cols);

						if (identityMatrix) {
							packMatrix(rpcOut, identityMatrix);
							pack(rpcOut, (unsigned char)MSG_OK);

							freeMatrix(identityMatrix);
						} else {
							pack(rpcOut, (unsigned char)MSG_NOK);
						}
					} else {
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
				} break;

				

				case opLeerMatriz:
				{
				 if (m) {
					matrix_t* matrixToWrite = unpackMatrix(rpcIn);

					 if (matrixToWrite) {

						p->writeMatrix(matrixToWrite, fileName);
						pack(rpcOut, (unsigned char)MSG_OK);

						freeMatrix(matrixToWrite);
					} else {
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
				} else {
					pack(rpcOut, (unsigned char)MSG_NOK);
				}
				}break;
			*/
				default:
				{
					std::cout << "Error: función no definida" << std::endl;
					pack(rpcOut, (unsigned char)MSG_NOK);
				}break;
			}

			sendMSG(clientId, rpcOut);
		};
};                          