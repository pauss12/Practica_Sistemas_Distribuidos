#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"


void sendMatrixOp(int serverId, const matrix_t& matrizA, const matrix_t& matrizB, operacionesEnum op) {
    std::vector<unsigned char> rpcOut;
    // Empaqueta  la operación
    pack(rpcOut, op);

    // Empaquetar las matrices
    packMatrix(rpcOut, matrizA.data, matrizA.rows, matrizA.cols);
    packMatrix(rpcOut, matrizB.data, matrizB.rows, matrizB.cols);

    // Envía la operación y las matrices al servidor
    sendMSG(serverId, rpcOut);
}


void recvMatrixOp(int serverId, matrix_t& matriz, const std::string& rutaArchivo, operacionesEnum op) {
    std::vector<unsigned char> rpcIn;
    std::vector<unsigned char> rpcOut;
    
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
}


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
};
/*
	 void multiplicarMatrices(matrix_t matrizA, matrix_t matrizB) {
			 
        sendMatrixOp(serverConnection.serverId, matrizA, matrizB, opMultiplicarMatrices);
    }

	matrix_t leerMatriz(const std::string& rutaArchivo) {
        matrix_t matriz;
        recvMatrixOp(serverConnection.serverId, matriz, rutaArchivo, opLeerMatriz);
        return matriz;
    }

    void escribirMatriz(const matrix_t& matriz, const std::string& rutaArchivo) {
        sendMatrixOp(matriz, rutaArchivo, opEscribirMatriz);
    }

    matrix_t crearIdentidad(int filas, int columnas) {
        sendMatrixSizeOp(filas, columnas, opCrearIdentidad);
        matrix_t matriz;
        recvMatrixOp(matriz, "", opCrearIdentidad);
        return matriz;
    }

    matrix_t crearRandom(int filas, int columnas, int rangoMin, int rangoMax) {
        sendRandomMatrixOp(serverConnection.serverId, filas, columnas, rangoMin, rangoMax, opCrearRandom);
        matrix_t matriz;
        recvMatrixOp(serverConnection.serverId, matriz, "", opCrearRandom);
        return matriz;
    }
*/