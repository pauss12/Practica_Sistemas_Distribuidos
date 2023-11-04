
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"

#include <thread>	
#include "matrix_imp.h"

void atiendeCliente(int idCliente){
	
	//Crear interfaz de servidor
	matrix_imp imp = matrix_imp(idCliente);
	//Mientras no cerre conexion
	do{
		//Atiende operacion
		imp.recibeOp();
		
	}while(!imp.connectionClosed());
	//Va a estar en el while hasta que se invoque al destructor
}


int main(int argc, char** argv)
{
	//iniciar un servidor
    auto serverSocket=initServer(60000);
    while(1){
		//esperar conexiones en puerto
         while(!checkClient()){
             usleep(1000);
        }
		
		//resolver identificador Cliente
       int clientId=getLastClientID();
	   std::thread *th = new std::thread(atiendeCliente, clientId);
   }

    close(serverSocket);
    return 0;
}
