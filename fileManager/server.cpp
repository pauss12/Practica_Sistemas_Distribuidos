
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include <thread>	
#include "filemanagerimp.h"

void atiendeCliente(int clientId){
	
	//Crear interfaz de servidor
	filemanager_imp = filemanager_imp(clientId);
	//Mientras no cerre conexion
	do{
		//Atiende operacion
		imp.recibeOp();
		
	}while(!imp.connectionClosed());
	//Va a estar en el while hasta que se invoque al destructor
};


int main(int argc, char** argv)
{
	//iniciar un servidor
    auto serverSocket=initServer(60000);
    while(1){
		//esperar conexiones en puerto
         while(!checkClient()){
             usleep(100);
        }
		
		//resolver identificador Cliente
       int clientId=getLastClientID();
	   std::thread *th = new std::thread(atiendeCliente, clientId);
	   th->join();
   }
    close(serverSocket);
    return 0;
}