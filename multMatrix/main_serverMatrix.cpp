
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"

#include <thread>	
#include "matrix_imp.h"

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
