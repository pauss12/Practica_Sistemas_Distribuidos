#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

//CLASE IMP (SERVER) --------------------------------------------------------------------------------------------------
class matrix_imp{

	private: 
		int clientId = -1;
		multMatrix *matriz_server = nullptr;

	public:
	
		matrix_imp(int clientId) : clientId(clientId) {};
		~matrix_imp() { delete matriz_server; };

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
					delete matriz_server;
					matriz_server = nullptr;
					pack(rpcOut, (unsigned char)MSG_OK);
				}break;
	
				case opLeerMatriz:
				{

					if (!matriz_server) {
						std::cout << "La instancia de multMatrix no está creada" << std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
						break;
					}

					std::string dato;

					// Recibo la cadena con su tamaño y contenido; la operacion ya ha sido desempaquetada antes de entrar en el switch
					//desempaquetar el tamaño de la cadena y su contenido
					int tam = unpack<int>(rpcIn);
					dato.resize(tam);
					unpackv(rpcIn, (char *)dato.data(), tam);

					// Llamar a la funcion para que lea la matriz en el server
					matrix_t *mmatriz = matriz_server->readMatrix(dato.c_str());

					// Empaquetar el ok
					pack(rpcOut, (unsigned char)MSG_OK);

					// empaquetar la matriz
					packMatrix(rpcOut, mmatriz->data, mmatriz->rows, mmatriz->cols);

					//Eliminar la matriz y liberar la memoria
					delete[] mmatriz->data;
					delete mmatriz;

				}break;

				case opEscribirMatriz:
				{
					if (!matriz_server) {
						std::cout << "La instancia de multMatrix no está creada" << std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
						break;
					}

					std::string dato;

					// Desempaquetar la matriz y la ruta del archivo
					matrix_t *mmatriz = new matrix_t();
					mmatriz->rows = unpack<int>(rpcIn);

					mmatriz->cols = unpack<int>(rpcIn);

					mmatriz->data = new int[mmatriz->rows * mmatriz->cols];

					unpackv(rpcIn, mmatriz->data, mmatriz->rows * mmatriz->cols);

					// Recibo la cadena con su tamaño y contenido; la operacion ya ha sido desempaquetada antes de entrar en el switch
					//desempaquetar el tamaño de la cadena y su contenido
					int tam = unpack<int>(rpcIn);

					dato.resize(tam);
					
					unpackv(rpcIn, (char *)dato.data(), tam);

					// Llamar a la funcion para que escriba la matriz en el server
					matriz_server->writeMatrix(mmatriz, dato.c_str());

					// Empaquetar el ok
					pack(rpcOut, (unsigned char)MSG_OK);

					//Eliminar la matriz y liberar la memoria
					delete[] mmatriz->data;
					delete mmatriz;
					
				}break;

				case opCrearIdentidad:
				{
					int rows = unpack<int>(rpcIn);
					int cols = unpack<int>(rpcIn);

					if (rows > 0 && cols > 0)
					{
						matrix_t *identityMatrix = matriz_server->createIdentity(rows, cols);

						if (identityMatrix)
						{
							packMatrix(rpcOut, identityMatrix->data, identityMatrix->rows, identityMatrix->cols);
							pack(rpcOut, (unsigned char)MSG_OK);

							// Libera la memoria de la matriz identidad
							delete[] identityMatrix->data;
							delete identityMatrix;

						}else{
							pack(rpcOut, (unsigned char)MSG_NOK);
						}

					}else{
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