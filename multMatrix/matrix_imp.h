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
					// Comprobar que existe una instancia de la matriz del server para poder acceder a las funciones
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
					// Comprobar que existe una instancia de la matriz del server para poder acceder a las funciones
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
					// Comprobar que existe una instancia de la matriz del server para poder acceder a las funciones
					if (!matriz_server)
					{
						std::cout << "La instancia de multMatrix no está creada" << std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
						break;
					}

					int rows = unpack<int>(rpcIn);
					int cols = unpack<int>(rpcIn);

					if (rows > 0 && cols > 0)
					{
						matrix_t *identityMatrix = matriz_server->createIdentity(rows, cols);

						if (identityMatrix)
						{
							pack(rpcOut, (unsigned char)MSG_OK);
							packMatrix(rpcOut, identityMatrix->data, identityMatrix->rows, identityMatrix->cols);
							
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

				
				case opCrearRandom:
				{
					// Comprobar que existe una instancia de la matriz del server para poder acceder a las funciones
					if (!matriz_server)
					{
						std::cout << "La instancia de multMatrix no está creada" << std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
						break;
					}

					int rows = unpack<int>(rpcIn);
					int cols = unpack<int>(rpcIn);

					if (rows > 0 && cols > 0)
					{
						matrix_t *randomMatrix = matriz_server->createRandMatrix(rows, cols);

						if (randomMatrix)
						{
							pack(rpcOut, (unsigned char)MSG_OK);
							packMatrix(rpcOut, randomMatrix->data, randomMatrix->rows, randomMatrix->cols);

							// Libera la memoria de la matriz identidad
							delete[] randomMatrix->data;
							delete randomMatrix;

						}else{
							pack(rpcOut, (unsigned char)MSG_NOK);
						}

					}else{
						pack(rpcOut, (unsigned char)MSG_NOK);
					}

				}break;

				case opMultiplicarMatrices:
				{
					//Comprobar que existe una instancia de la matriz del server para poder acceder a las funciones 
					if (!matriz_server){
						std::cout << "La instancia de multMatrix no está creada" << std::endl;
						pack(rpcOut, (unsigned char)MSG_NOK);
						break;
					}

					// Crear las dos matrices en las que se va a guardar las dos matrices
					matrix_t *matriz_1 = new matrix_t();
					matrix_t *matriz_2 = new matrix_t();

					//Desempaquetar la primera matriz que me mandan desde el cliente
					matriz_1->rows = unpack<int>(rpcIn);
					matriz_1->cols = unpack<int>(rpcIn);
					matriz_1->data = new int[matriz_1->rows * matriz_1->cols];

					unpackv(rpcIn, matriz_1->data, matriz_1->rows * matriz_1->cols);

					// Desempaquetar la segunda matriz que me mandan desde el cliente
					matriz_2->rows = unpack<int>(rpcIn);
					matriz_2->cols = unpack<int>(rpcIn);
					matriz_2->data = new int[matriz_2->rows * matriz_2->cols];

					unpackv(rpcIn, matriz_2->data, matriz_2->rows * matriz_2->cols);

					// Multiplicar las dos matrices
					matrix_t *matriz_resultado = matriz_server->multMatrices(matriz_1, matriz_2);

					// Empaquetar el ok
					pack(rpcOut, (unsigned char)MSG_OK);

					//Empaquetar la matriz resultado
					packMatrix(rpcOut, matriz_resultado->data, matriz_resultado->rows, matriz_resultado->cols);

					//Eliminar la matriz y liberar la memoria
					delete[] matriz_1->data;
					delete[] matriz_2->data;
					delete matriz_1;
					delete matriz_2;
					
					delete[] matriz_resultado->data;
					delete matriz_resultado;

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