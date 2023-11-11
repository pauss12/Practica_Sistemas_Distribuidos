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
        int clientId = -1;
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
                        
                        pack(rpcOut, (unsigned char)MSG_OK);

                        int fileCount = fileList->size();

                        fileList->reserve(fileCount);

                        pack(rpcOut, fileCount);
        
                        // Empaqueta los nombres de los archivos.
                        for (int i = 0; i < fileCount; i++) {
                            std::string *fileName = fileList->at(i);
                            int fileNameLength = fileName->length() + 1;
                            pack(rpcOut, fileNameLength);
                            packv(rpcOut, (char *)fileName->data(), fileNameLength);
                        }

                        fileManager->freeListedFiles(fileList);

                    } else {
                        // No hay una instancia de FileManager para realizar la operación, envía un error.
                        std::cout<<"Error: no hay una instancia de FileManager\n";

                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }

                    std::cout << "--------------------------------------------" << "\n";
                    
                }break;

                case opReadFile:
                {
                    if (fileManager != nullptr) {

                        std::string fileName;
                        int fileNameLength = unpack<int>(rpcIn);
                        fileName.resize(fileNameLength);
                        unpackv(rpcIn, (char *)fileName.data(), fileNameLength);

                        char *data = nullptr;
                        unsigned long int dataLength = 0;
                        fileManager->readFile((char *)fileName.data(), data, dataLength);

                        pack(rpcOut, (unsigned char)MSG_OK);

                        pack(rpcOut, dataLength);
                        packv(rpcOut, data, dataLength);

                        delete[] data;

                    } else {

                        std::cout<<"Error: no hay una instancia de FileManager\n" << std::endl;

                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }

                }break;

                case opWriteFile:
                {
                    if (fileManager != nullptr) {

                        std::cout << "OPERACION WRITE FILE" << "\n" << std::endl;

                        // Recibir el nombre del archivo a escribir.
                        std::string fileName;

                        int fileNameLength = unpack<int>(rpcIn);
                        fileName.resize(fileNameLength);

                        unpackv(rpcIn, (char *)fileName.data(), fileNameLength);

                        std::cout << "Nombre del archivo: " << fileName << "\n" << std::endl;

                        // Recibir el contenido del archivo.
                        unsigned long int dataLength = unpack<unsigned long int>(rpcIn);
                        char *data = new char[dataLength];

                        for (unsigned long int i = 0; i < dataLength; i++) {
                            data[i] = rpcIn[i];
                        }

                        std::cout << "Contenido del archivo: " << data << "\n" << std::endl;
                        
                        // Comprobar si el fileName no es nulo y si el data no es nulo
                        if (fileName.empty() || data == nullptr)
                        {
                            std::cout << "Error: el nombre del fichero o el contenido del fichero es nulo\n";
                            pack(rpcOut, (unsigned char)MSG_NOK);
                            return;
                        }

                        //llamar a la funcion writeFile
                        fileManager->writeFile((char *)fileName.data(), data, dataLength);

                        std::cout << "Archivo escrito" << "\n" << std::endl;

                        // Empaqueta la respuesta.
                        pack(rpcOut, (unsigned char)MSG_OK);

                        std::cout << "Empaquetado la operacion" << "\n" << std::endl;

                        std::cout << "--------------------------------------------" << "\n" << std::endl;

                    } else {
                        // No hay una instancia de FileManager para realizar la operación, envía 
                        //un error.
                        std::cout<<"Error: no hay una instancia de FileManager\n" << std::endl;

                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }
                }break;

                default:
                {
                    // Operación no definida, envía un error.
                    std::cout<<"Error: función no definida\n" << std::endl;
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }break;
            }

            // Envía la respuesta al cliente.
            sendMSG(clientId, rpcOut);
        };
};
