#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operacionesFiles.h"
#include "filemanager.h"

//MIRAR LINEA 55
/*

  void sendStringOp(int serverId, const std::string& dato, FileManagerOp op) {
        std::vector<unsigned char> rpcOut;
        std::vector<unsigned char> rpcIn;

        pack(rpcOut, op);

        int tam = dato.length() + 1;
        pack(rpcOut, tam);
        packv(rpcOut, dato.c_str(), tam);

        sendMSG(serverId, rpcOut);
        recvMSG(serverId, rpcIn);

        if (rpcIn[0] != MSG_OK) {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
    }


    void recvStringOp(int serverId, std::string& dato, FileManagerOp op) {
        std::vector<unsigned char> rpcOut;
        std::vector<unsigned char> rpcIn;

        pack(rpcOut, op);
        sendMSG(serverId, rpcOut);

        recvMSG(serverId, rpcIn);
        unsigned char ok = unpack<unsigned char>(rpcIn);

        if (ok != MSG_OK) {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        } else {
            int tam = unpack<int>(rpcIn);
            dato.resize(tam);
            unpackv(rpcIn, (char*)dato.data(), tam);
        }
    }
*/

class FileManager_Stub
{
    private:
        //std::string ip = "54.157.164.116";
        std::string ip = "127.0.0.1";
        int port = 60000;
        connection_t serverConnection;

    public:

        FileManager_Stub(std::string path) {

            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            FileManagerOp op = opConstructor;

            serverConnection = initClient(ip, port);

            pack(rpcOut, op);

            int tam = path.length() + 1;
            pack(rpcOut, tam);
            packv(rpcOut, path.c_str(), tam);

            sendMSG(serverConnection.serverId, rpcOut);

            std::cout << "Enviando constructor\n";

            recvMSG(serverConnection.serverId, rpcIn);

            if (rpcIn[0] != MSG_OK) {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
            }
        };

        std::vector<std::string> *listFiles() {

            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, opListFiles);

            std::cout << "Ya he empaquetado la operacion\n";

            //Mandar la operacion que el cliente quiere hacer
            sendMSG(serverConnection.serverId, rpcOut);

            std::cout << "Enviando listFiles\n";

            //recibiendo la respuesta
            recvMSG(serverConnection.serverId, rpcIn);

            std::cout << "Recibiendo respuesta\n";

            if (rpcIn[0] != MSG_OK) {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
                return nullptr;
            }
                
            // Desempaquetar la lista de archivos y agregarlos al vector 'fileList.'
            int numFiles = unpack<int>(rpcIn);

            std::cout << "Numero de ficheros: " << numFiles << "\n";

            //Creo un vector de strings
            std::vector<std::string*> *fileList = new std::vector<std::string*>[numFiles];

            std::cout << "Creo el vector de strings\n";

            //Desempaqueto la respuesta

            for (int i = 0; i < numFiles; i++) {
                
                int fileNameLen = unpack<int>(rpcIn);
                char fileName[fileNameLen];
                unpackv(rpcIn, fileName, fileNameLen);
                fileList->push_back(fileName);
            }   
           
            return fileList;
        };
        /*
                   void readFile(const std::string& fileName, char* &data, unsigned long int &dataLength) {
                       std::vector<unsigned char> rpcOut;
                       std::vector<unsigned char> rpcIn;

                       pack(rpcOut,opReadFile);
                       sendStringOp(serverConnection.serverId, fileName,opReadFile);


                       recvMSG(serverConnection.serverId, rpcIn);

                       if (rpcIn[0] != MSG_OK) {
                           std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
                       } else {
                           dataLength = unpack<unsigned long int>(rpcIn);
                           data = new char[dataLength];

                           recvMSG(serverConnection.serverId, rpcIn);
                           for (unsigned long int i = 0; i < dataLength; i++) {
                               data[i] = rpcIn[i];
                           }
                       }
                   };

                   void writeFile(const std::string& fileName, const char* data, unsigned long int dataLength) {
                       std::vector<unsigned char> rpcOut;

                       pack(rpcOut,opWriteFile);
                       sendStringOp(serverConnection.serverId, fileName,opWriteFile);

                       pack(rpcOut, dataLength);
                       for (unsigned long int i = 0; i < dataLength; i++) {
                           rpcOut.push_back(data[i]);
                       }

                       sendMSG(serverConnection.serverId, rpcOut);
                   }

                   void freeListedFiles(std::vector<std::string>* fileList) {
                       delete fileList;
                   };
           */
};