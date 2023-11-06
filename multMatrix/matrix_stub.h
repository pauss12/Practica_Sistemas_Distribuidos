#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

// ENVIAR UNA MATRIZ ---------------------------------------------------------------------------------------------------------
void sendMatrixOp(int id, operacionesEnum op, matrix_t &matrix)
{

	std::vector<unsigned char> rpcOut;
	// Empaquetar la operación
	pack(rpcOut, op);

	// Empaquetar el número de filas y columnas
	pack(rpcOut, matrix.rows);
	pack(rpcOut, matrix.cols);

	// Empaquetar los datos de la matriz
	for (int i = 0; i < matrix.rows; i++)
	{
		for (int j = 0; j < matrix.cols; j++)
		{
			pack(rpcOut, matrix.data[i * matrix.cols + j]);
		}
	}

	// Envía la operación y la matriz al servidor
	sendMSG(id, rpcOut);
}

// RECIBIR UNA MATRIZ --------------------------------------------------------------------------------------
matrix_t *recvMatrix(int id, const std::string &rutaArchivo, operacionesEnum op)
{
	std::vector<unsigned char> rpcIn;
	std::vector<unsigned char> rpcOut;

	// Matriz que vamos a devolver
	matrix_t *matriz;

	// Empaqueta la operación
	pack(rpcOut, op);

	if (op == opLeerMatriz)
	{
		pack(rpcOut, static_cast<int>(rutaArchivo.size()));
		packv(rpcOut, rutaArchivo.data(), rutaArchivo.size());
	}

	// Envía la operación y otros parámetros al servidor
	sendMSG(id, rpcOut);

	// Recibe la respuesta del servidor
	recvMSG(id, rpcIn);

	// Desempaqueta la matriz
	int rows, cols;

	rows = unpack<int>(rpcIn);
	cols = unpack<int>(rpcIn);

	matriz->rows = rows;
	matriz->cols = cols;
	matriz->data = new int[rows * cols];

	for (int i = 0; i < rows * cols; i++)
	{
		matriz->data[i] = unpack<int>(rpcIn);
	}
	return matriz;
}

//CLASE MULTMATRIX DEL CLIENTE -----------------------------------------------------------------
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
			
			std::cout<<"EL CONSTRUCTOR DEL CLIENTE FUNCIONA"<<std::endl;
			
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
			
			std::cout<<"EL DESTRUCTOR DEL CLIENTE FUNCIONA"<<std::endl;
			
			if (rpcIn[0] != MSG_OK)
				std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";
			
			close(serverConnection.serverId);
		};

		// Leer la matriz
		matrix_t *leerMatriz(const std::string &rutaArchivo)
		{

			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;

			// Enviar la rutaArchivo
			send_cadena(serverConnection.serverId, rutaArchivo, opLeerMatriz);

			// Comprobar que se ha mandado bien
			recvMSG(serverConnection.serverId, rpcIn);

			if (rpcIn[0] != MSG_OK)
				std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

			// Creo la matriz para recibir lo que me mande el servidor
			matrix_t *matriz;

			matriz = recvMatrix(serverConnection.serverId, rutaArchivo, opLeerMatriz);

			return matriz;
		};
};

/*
	multMatrix_stub multiplicarMatrices(const multMatrix_stub& matrizA, const multMatrix_stub& matrizB) {
		// Empaquetar las matrices A y B
		sendMatrixOp(matrizA, opMultiplicarMatrices);
		sendMatrixOp(matrizB, opMultiplicarMatrices);

		// Comprobar que se han enviado bien
		recvMSG(serverConnection.serverId, rpcIn);

		if (rpcIn[0] != MSG_OK)
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";

		// Crear la matriz para recibir el resultado del servidor
		multMatrix_stub resultado;
		resultado = recvMatrixOp(serverConnection.serverId, "", opMultiplicarMatrices);

		return resultado;
	}


	void escribirMatriz(const multMatrix_stub& matriz, const std::string& rutaArchivo) {
		// Empaquetar la matriz y la ruta del archivo
		sendMatrixOp(matriz, opEscribirMatriz);
		sendStringOp(serverConnection.serverId, rutaArchivo, opEscribirMatriz);

		// Comprobar que se han enviado bien
		recvMSG(serverConnection.serverId, rpcIn);

		if (rpcIn[0] != MSG_OK)
			std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
	}


	matrix_t crearIdentidad(int filas, int columnas) {
		return sendMatrixOp(opCrearIdentidad, filas, columnas, 0, 0, "");
}

	matrix_t crearRandom(int filas, int columnas, int rangoMin, int rangoMax) {
		return sendMatrixOp(opCrearRandom, filas, columnas, rangoMin, rangoMax, "");
	}

*/