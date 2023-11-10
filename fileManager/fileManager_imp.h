#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operacionesFiles.h"
#include "fileManager.h"

class FileManager_Imp {
private:
    int clientId =-1;
    FileManager *fileManager = nullptr;

public:

    FileManager_Imp(int clientId) : clientId(clientId){};

    bool connectionClosed() {
        return fileManager == nullptr;
    }

    void recibeOp() {

        std::vector<unsigned char> rpcIn;
        std::vector<unsigned char> rpcOut;

        // Recibe operación
        recvMSG(clientId, rpcIn);

        FileManagerOp operacion = unpack<FileManagerOp>(rpcIn);

        // Ejecuta la operación correspondiente y genera una respuesta.
        switch (operacion) {

            case opConstructor:
            {
                if (fileManager == nullptr)
                {
                    // Recibir el path del directorio a administrar.
                    std::string path;
                    int pathLength = unpack<int>(rpcIn);
                    path.resize(pathLength);
                    unpackv(rpcIn, (char *)path.data(), pathLength);

                    std::cout << "Path: " << path << "\n";

                    // Llamar al constructor del imp
                    fileManager = new FileManager(path);

                    pack(rpcOut, (unsigned char)MSG_OK);

                } else {

                    // Ya hay una instancia creada, envía un error.
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
                
            }break;

            case opDestructor:
            {

                if (fileManager != nullptr) {
                    delete fileManager;
                    std::cout<<"Se destruyo el file manager\n";
                    fileManager = nullptr;
                    pack(rpcOut, (unsigned char)MSG_OK);

                } else {
                    // No hay una instancia para destruir, envía un error.
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
            }break;

            case opListFiles:
            {
                if (fileManager != nullptr) {
                    
                    // Si el FileManager está inicializado, realiza la operación "listFiles".
                    std::vector<std::string*> *fileList = fileManager->listFiles();
                    
                    // Empaqueta la respuesta.
                    pack(rpcOut, (unsigned char)MSG_OK);

                    std::cout << "Empaquetado la operacion" << "\n";

                    // Empaqueta la lista de archivos.
                    int fileCount = fileList->size();

                    //Reservar tamaño para el numero de ficheros de fileList
                    fileList->reserve(fileCount);

                    std::cout << "Numero de ficheros: " << fileCount << "\n";

                    pack(rpcOut, fileCount);

                    std::cout << "Empaquetado el numero de ficheros" << "\n";
                    
                    // Empaqueta los nombres de los archivos.
                    for (int i = 0; i < fileCount; i++) {
                        std::string *fileName = fileList->at(i);
                        //int fileNameLength = fileName->size();
                        int fileNameLength = fileName->length() + 1;
                        pack(rpcOut, fileNameLength);
                        packv(rpcOut, (char *)fileName->data(), fileNameLength);
                    }

                    std::cout << "Empaquetado la lista de ficheros" << "\n";

                    // Libera la memoria de la lista de archivos.
                    fileManager->freeListedFiles(fileList);

                    std::cout << "Liberado la lista de ficheros" << "\n";

                } else {
                    // No hay una instancia de FileManager para realizar la operación, envía 
                    //un error.
                    std::cout<<"Error: no hay una instancia de FileManager\n";

                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
                
            }break;
            

            default:
            {
                // Operación no definida, envía un error.
                std::cout<<"Error: función no definida\n";
                pack(rpcOut, (unsigned char)MSG_NOK);
            }break;
        }

        // Envía la respuesta al cliente.
        sendMSG(clientId, rpcOut);
    };
};
