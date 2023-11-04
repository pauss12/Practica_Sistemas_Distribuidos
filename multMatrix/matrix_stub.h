#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"
#include "multmatrix.h"

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
			
			std::cout<<"EL CONSTRUCTOR FUNCIONA"<<std::endl;
			
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
};

	 void multiplicarMatrices(matrix_t matrizA, matrix_t matrizB) {
        sendMatrixOp(serverConnection.serverId, matrizA, matrizB, multiplicarMatricesOp);
    }

	matrix_t leerMatriz(const std::string& rutaArchivo) {
        matrix_t matriz;
        recvMatrixOp(serverConnection.serverId, matriz, rutaArchivo, leerMatrizOp);
        return matriz;
    }

    void escribirMatriz(const matrix_t& matriz, const std::string& rutaArchivo) {
        sendMatrixOp(serverConnection.serverId, matriz, rutaArchivo, escribirMatrizOp);
    }

    matrix_t crearIdentidad(int filas, int columnas) {
        sendMatrixSizeOp(serverConnection.serverId, filas, columnas, crearIdentidadOp);
        matrix_t matriz;
        recvMatrixOp(serverConnection.serverId, matriz, "", crearIdentidadOp);
        return matriz;
    }

    matrix_t crearRandom(int filas, int columnas, int rangoMin, int rangoMax) {
        sendRandomMatrixOp(serverConnection.serverId, filas, columnas, rangoMin, rangoMax, crearRandomOp);
        matrix_t matriz;
        recvMatrixOp(serverConnection.serverId, matriz, "", crearRandomOp);
        return matriz;
    }