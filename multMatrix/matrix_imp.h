#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

class matrix_imp{

	private: 
		int clientId = -1;
		multMatrix* m = nullptr;

	public:
	
		matrix_imp(int clientId) : clientId(clientId) {};

		bool connectionClosed() {
			return m == nullptr;
		}

		void recibeOp() {
			
			std::vector<unsigned char> rpcIn;
			std::vector<unsigned char> rpcOut;
			
			recvMSG(clientId, rpcIn);

			operacionesEnum operacion = unpack<operacionesEnum>(rpcIn);

			switch (operacion) {
				
				case opConstructor:
				{
					m = new multMatrix();
					pack(rpcOut, (unsigned char)MSG_OK);
					
				}break;
					
				case opDestructor:
				{
					if (m) {
						
						delete m;
						pack(rpcOut, (unsigned char)MSG_OK);
						
					} else {
						std::cout << "La instancia de multMatrix no está creada" << "\n"<< std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
					
				}break;
	
				case opLeerMatriz:
				{
					if (m) {
						//const char* fileName =  Obtener el nombre del archivo de algún lugar ;
						matrix_t* result = m->readMatrix(fileName);
						if (result) {
							// Realizar operaciones con la matriz leída
							// Por ejemplo, puedes almacenar 'result' en una variable miembro de la clase
							pack(rpcOut, (unsigned char)MSG_OK);
						} else {
							std::cout << "Error al leer la matriz desde el archivo" << "\n"<< std::endl;
							pack(rpcOut, (unsigned char)MSG_NOK);
						}
					} else {
						std::cout << "La instancia de multMatrix no está creada" << "\n"<< std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
					
				}break;
					
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

				case opCrearRandom:
				{
					int rows = unpack<int>(rpcIn);
					int cols = unpack<int>(rpcIn);

					if (rows > 0 && cols > 0) {  
						matrix_t* randomMatrix = p->createRandMatrix(rows, cols);

						if (randomMatrix) {
							packMatrix(rpcOut, randomMatrix);
							pack(rpcOut, (unsigned char)MSG_OK);

							// Libera la memoria de la matriz aleatoria
							freeMatrix(randomMatrix);
						} else {
							pack(rpcOut, (unsigned char)MSG_NOK);
						}
					} else {
						pack(rpcOut, (unsigned char)MSG_NOK);
					}
				}break;

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
				
				default:
				{
					std::cout << "Error: función no definida" << std::endl;
					pack(rpcOut, (unsigned char)MSG_NOK);
				}break;
			}

				sendMSG(clientId, rpcOut);
		};
};                          