#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operacionesFiles.h"
#include "filemanager.h"

class FileManager_Imp {

    private:
        int clientId = -1;
        FileManager *fileManager = nullptr;

    public:

        //Constructor de fileManager_Imp
        FileManager_Imp(int clientId) : clientId(clientId){};

        //Destructor de fileManager_Imp
        ~FileManager_Imp() { delete fileManager; };

        bool connectionClosed() {
            return fileManager == nullptr;
        }

        void recibeOp() {

            std::vector<unsigned char> rpcIn;
            std::vector<unsigned char> rpcOut;

            recvMSG(clientId, rpcIn);

            FileManagerOp operacion = unpack<FileManagerOp>(rpcIn);

            switch (operacion) {

                case opConstructor:
                {
                    if (fileManager == nullptr)
                    {
                        std::string path;

                        int pathLength = unpack<int>(rpcIn);
                        path.resize(pathLength);

                        unpackv(rpcIn, (char *)path.data(), pathLength);

                        fileManager = new FileManager(path);

                        pack(rpcOut, (unsigned char)MSG_OK);

                    } else {

                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }
                    
                }break;

                case opDestructor:
                {

                    delete fileManager;
                    fileManager = nullptr;
                    pack(rpcOut, (unsigned char)MSG_OK);

                }break;

                case opListFiles:
                {
                    if (fileManager != nullptr) {

                        std::vector<std::string*> *fileList = fileManager->listFiles();
                        
                        pack(rpcOut, (unsigned char)MSG_OK);

                        int fileCount = fileList->size();

                        fileList->reserve(fileCount);

                        pack(rpcOut, fileCount);
        
                        // Empaqueta los nombres de los archivos.
                        for (int i = 0; i < fileCount; i++)
                        {

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
                    
                }break;

                case opReadFile:
                {
                    if (fileManager != nullptr)
                    {

                        int fileNameLength = unpack<int>(rpcIn);

                        char *fileName = new char[fileNameLength];

                        unpackv(rpcIn, fileName, fileNameLength);

                        char *data = nullptr;
                        unsigned long int dataLength = 0;

                        fileManager->readFile(fileName, data, dataLength);

                        pack(rpcOut, (unsigned char)MSG_OK);

                        pack(rpcOut, dataLength);

                        packv(rpcOut, data, dataLength);

                        delete[] data;
                    }
                    else
                    {

                        std::cout << "Error: no hay una instancia de FileManager\n" << std::endl;
                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }
                }
                break;

                case opWriteFile:
                {
                    if (fileManager != nullptr) 
                    {

                        std::string fileName;

                        int fileNameLength = unpack<int>(rpcIn);
                        fileName.resize(fileNameLength);

                        unpackv(rpcIn, (char *)fileName.data(), fileNameLength);

                        unsigned long int dataLength = unpack<unsigned long int>(rpcIn);
                        char *data = new char[dataLength];

                        unpackv(rpcIn, data, dataLength);

                        // Comprobar si el fileName no es nulo y si el data no es nulo
                        if (fileName.empty() || data == nullptr)
                        {
                            std::cout << "Error: el nombre del fichero o el contenido del fichero es nulo\n";
                            pack(rpcOut, (unsigned char)MSG_NOK);
                            return ;
                        }

                        fileManager->writeFile((char *)fileName.data(), data, dataLength);

                        pack(rpcOut, (unsigned char)MSG_OK);

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
