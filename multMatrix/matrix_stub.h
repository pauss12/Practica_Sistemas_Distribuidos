#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"


// CLASE MULTMATRIX DEL CLIENTE -----------------------------------------------------------------
class multMatrix_stub
{
	private:
		//std::string ip = "172.31.51.119";
		std::string ip = "127.0.0.1";
		int port = 60000;
		connection_t serverConnection;
		
	public:
	
		multMatrix_stub()
		{
			serverConnection = initClient(ip, port);
			
			operacionesEnum op = opConstructor;
			
			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;
			
			pack(rpcOut, op);

			sendMSG(serverConnection.serverId, rpcOut);
			
			//Recibir OK (0: no okey, 1: Okey)
			recvMSG(serverConnection.serverId, rpcIn);
			
			if (rpcIn[0] != MSG_OK)
				std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";
		};
		
		//Lo q hace el destructor es cerrar la conexion
		~multMatrix_stub(){
			
			operacionesEnum op = opDestructor;
			
			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;
			
			pack(rpcOut, op);

			sendMSG(serverConnection.serverId, rpcOut);
			
			//Recibir OK (0: no okey, 1: Okey)
			recvMSG(serverConnection.serverId, rpcIn);
			
			if (rpcIn[0] != MSG_OK)
				std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";
			
			close(serverConnection.serverId);
		};

		// Leer la matriz
		matrix_t *leerMatriz(const std::string &rutaArchivo)
		{
			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;

			matrix_t *matriz = new matrix_t();

			// Enviar la rutaArchivo
			send_cadena(serverConnection.serverId, rutaArchivo, opLeerMatriz);

			recvMSG(serverConnection.serverId, rpcIn);

			//Desempaquetar el ok
			unsigned char ok = unpack<unsigned char>(rpcIn);

			if (ok != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

			//Desempaquetar las filas y columnas de la matriz
			matriz->rows = unpack<int>(rpcIn);
			matriz->cols = unpack<int>(rpcIn);
			matriz->data = new int[matriz->rows * matriz->cols];

			//Desempaquetar los datos de la matriz
			unpackv(rpcIn, matriz->data, matriz->rows * matriz->cols);

			return matriz;
		};

		
		// Escribir la matriz
		void escribirMatriz(matrix_t *matriz, const std::string &rutaArchivo)
		{
			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;
			
			//Empaquetar la operacion
			pack(rpcOut, opEscribirMatriz);

			//Empaquetar la matriz
			packMatrix(rpcOut, matriz->data, matriz->rows, matriz->cols);

			//Empaquetar la ruta del archivo
			int tam = rutaArchivo.length() + 1;

			pack(rpcOut, tam);
			
			packv(rpcOut, rutaArchivo.data(), tam);

			sendMSG(serverConnection.serverId, rpcOut);		

			// Comprobar que se han enviado bien
			recvMSG(serverConnection.serverId, rpcIn);

			unsigned char ok = unpack<unsigned char>(rpcIn);

			if (ok != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

		};

		//Crear la matriz identidad
		matrix_t *crearIdentidad(int filas, int columnas)
		{
			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;

			matrix_t *matrizPrueba = new matrix_t();

			pack(rpcOut, opCrearIdentidad);

			pack(rpcOut, filas);
			pack(rpcOut, columnas);

			sendMSG(serverConnection.serverId, rpcOut);

			recvMSG(serverConnection.serverId, rpcIn);

			unsigned char ok = unpack<unsigned char>(rpcIn);

			if (ok != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

			matrizPrueba->rows = unpack<int>(rpcIn);
			matrizPrueba->cols = unpack<int>(rpcIn);

			matrizPrueba->data = new int[matrizPrueba->rows * matrizPrueba->cols];
			unpackv(rpcIn, matrizPrueba->data, matrizPrueba->rows * matrizPrueba->cols);

			return matrizPrueba;
		};

		// Crear random
		matrix_t *crearRandom(int filas, int columnas)
		{

			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;

			matrix_t *matrizRandom = new matrix_t();

			pack(rpcOut, opCrearRandom);

			pack(rpcOut, filas);
			pack(rpcOut, columnas);

			sendMSG(serverConnection.serverId, rpcOut);

			recvMSG(serverConnection.serverId, rpcIn);

			unsigned char ok = unpack<unsigned char>(rpcIn);

			if (ok != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

			matrizRandom->rows = unpack<int>(rpcIn);
			matrizRandom->cols = unpack<int>(rpcIn);

			matrizRandom->data = new int[matrizRandom->rows * matrizRandom->cols];
			unpackv(rpcIn, matrizRandom->data, matrizRandom->rows * matrizRandom->cols);

			return matrizRandom;
		};

		// Multiplicar las matrices
		matrix_t *multiplicarMatrices(matrix_t *matrizA, matrix_t *matrizB)
		{

			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;

			matrix_t *resultado = new matrix_t();

			pack(rpcOut, opMultiplicarMatrices);

			packMatrix(rpcOut, matrizA->data, matrizA->rows, matrizA->cols);
			packMatrix(rpcOut, matrizB->data, matrizB->rows, matrizB->cols);

			sendMSG(serverConnection.serverId, rpcOut);

			recvMSG(serverConnection.serverId, rpcIn);

			unsigned char ok = unpack<unsigned char>(rpcIn);

			if (ok != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
			
			resultado->rows = unpack<int>(rpcIn);
			resultado->cols = unpack<int>(rpcIn);
			
			resultado->data = new int[resultado->rows * resultado->cols];
			unpackv(rpcIn, resultado->data, resultado->rows * resultado->cols);

			return resultado;
		};
};