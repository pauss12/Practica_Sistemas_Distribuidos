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
		
};

/*

//Multiplicar las matrices
		matrix_t *multiplicarMatrices(const multMatrix_stub &matrizA, const multMatrix_stub &matrizB)
		{
			// Empaquetar las matrices A y B
			sendMatrixOp(matrizA, opMultiplicarMatrices);
			sendMatrixOp(matrizB, opMultiplicarMatrices);

			// Comprobar que se han enviado bien
			recvMSG(serverConnection.serverId, rpcIn);

			if (rpcIn[0] != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

			// Crear la matriz para recibir el resultado del servidor
			matrix_t resultado;
			resultado = recvMatrixOp(serverConnection.serverId, "", opMultiplicarMatrices);

			return resultado;
		};

		//Crear random
		matrix_t *crearRandom(int filas, int columnas, int rangoMin, int rangoMax)
		{

			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;



			//return sendMatrixOp(opCrearRandom, filas, columnas, rangoMin, rangoMax, "");
		}

	


	matrix_t crearIdentidad(int filas, int columnas) {
		return sendMatrixOp(opCrearIdentidad, filas, columnas, 0, 0, "");
}



*/