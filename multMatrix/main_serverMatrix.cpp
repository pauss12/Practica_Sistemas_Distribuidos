
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"

#include <thread>	
#include "matrix_imp.h"

void atiendeCliente(int clientId){
	
	//Crear interfaz de servidor
	std::cout << "En el hilo " << std::this_thread::get_id() << " entra dentro de atiende cliente" << std::endl;
	matrix_imp matrix_imp = matrix_imp(clientId);
	//Mientras no cerre conexion
	do{
		std::cout << "En el hilo " << std::this_thread::get_id() << " recibeOp()" << std::endl;
		//Atiende operacion
		matrix_imp.recibeOp();
		
	}while(!matrix_imp.connectionClosed());
	//Va a estar en el while hasta que se invoque al destructor
	std::cout << "En el hilo " << std::this_thread::get_id() << " sale de atiendeCliente" << std::endl;
	th->join();
}


int main(int argc, char** argv)
{
	//iniciar un servidor
	std::cout<<"entra dentro del main del server"<<std::endl;
    auto serverSocket=initServer(60000);
	std::cout<<"He iniciado al server"<<std::endl;
    while(1){
		//esperar conexiones en puerto
         while(!checkClient()){
             usleep(100);
        }
		
		//resolver identificador Cliente
       int clientId=getLastClientID();
	   std::thread *th = new std::thread(atiendeCliente, clientId);
   }
    close(serverSocket);
    return 0;
}