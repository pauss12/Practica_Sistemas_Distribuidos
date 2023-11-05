#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"


// Función para enviar una operación de matriz al servidor
void sendMatrixOp(int serverId, operacionesEnum op, const matrix_t& matrix) {
    
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
    sendMSG(serverId, rpcOut);
}

//RECIBIR UNA MATRIZ --------------------------------------------------------------------------------------
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

//MANDAR UNA CADENA ---------------------------------------------------------------------
void sendStringOp(int serverId, std::string dato, personaOp op)
{
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;
			
	pack(rpcOut, op);
			
	int tam = dato.length() + 1;
	pack(rpcOut, tam);
	packv(rpcOut, dato.data(), tam);
		
	sendMSG(serverId, rpcOut);
			
			//Recibir OK (0: no okey, 1: Okey)
	recvMSG(serverId, rpcIn);
	if (rpcIn[0] != MSG_OK)
		std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";	
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
		
		//Leer la matriz
		multMatrix_stub leerMatriz(const std::string& rutaArchivo) {

			//Enviar la rutaArchivo
			sendStringOp(serverConnection.serverId, rutaArchivo, opLeerMatriz);
			
			//Comprobar que se ha mandado bien
			recvMSG(serverConnection.serverId, rpcIn);
			
			if (rpcIn[0] != MSG_OK)
				std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";

			//Creo la matriz para recibir lo que me mande el servidor
			multMatrix_stub matriz;
			matriz = recvMatrixOp(serverConnection.serverId, rutaArchivo, opLeerMatriz);
			
			return matriz;
		};
};


/*
	void multiplicarMatrices(matrix_t matrizA, matrix_t matrizB) {
			 
        sendMatrixOp(serverConnection.serverId, matrizA, matrizB, opMultiplicarMatrices);
    }

	

    void escribirMatriz(const matrix_t& matriz, const std::string& rutaArchivo) {
        sendMatrixOp(matriz, rutaArchivo, opEscribirMatriz);
    }

  	matrix_t crearIdentidad(int filas, int columnas) {
    	return sendMatrixOp(opCrearIdentidad, filas, columnas, 0, 0, "");
}

	matrix_t crearRandom(int filas, int columnas, int rangoMin, int rangoMax) {
		return sendMatrixOp(opCrearRandom, filas, columnas, rangoMin, rangoMax, "");
	}

*/