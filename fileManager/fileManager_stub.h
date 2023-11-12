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

        //CONSTRUCTOR DEL CLIENTE ----------------------
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

        //DESTRUCTOR DEL CLIENTE ---------------------------
        ~FileManager_Stub()
        {

            FileManagerOp op = opDestructor;

            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);

            sendMSG(serverConnection.serverId, rpcOut);

            recvMSG(serverConnection.serverId, rpcIn);

            if (rpcIn[0] != MSG_OK)
            {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n" << std::endl;
            }

            close(serverConnection.serverId);
        };

        //LISTAR FICHEROS --------------------------------------
        std::vector<std::string*> *listFiles() {

            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, opListFiles);

            //Mandar la operacion que el cliente quiere hacer
            sendMSG(serverConnection.serverId, rpcOut);

            //recibiendo la respuesta
            recvMSG(serverConnection.serverId, rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn);

            if (ok != MSG_OK) {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n" << std::endl;
                return nullptr;
            }
                
            // Desempaquetar la lista de archivos y agregarlos al vector 'fileList.'
            int numFiles = unpack<int>(rpcIn);

            //Creo un vector de strings
            std::vector<std::string *> *fileList = new std::vector<std::string*>;
            fileList->reserve(numFiles + 1);

            //Desempaqueto la respuesta
            for (int i = 0; i < numFiles; i++)
            {
                int fileNameLen = unpack<int>(rpcIn);

                char fileName[fileNameLen];

                unpackv(rpcIn, fileName, fileNameLen);

                // Create a new string object and add it to the vector.
                fileList->push_back(new std::string(fileName));

            }
            return fileList;
        };

        // READ FILE ---------
        void readFile(std::string &fileName, char *data, unsigned long int dataLength)
        {
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, opReadFile);

            //Empaquetar el nombre del fichero que se quiere leer
            int tam = fileName.length() + 1;
            pack(rpcOut, tam);
            packv(rpcOut, fileName.data(), tam);

            sendMSG(serverConnection.serverId, rpcOut);

            //Recibir el ok y la informacion del fichero
            recvMSG(serverConnection.serverId, rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn);

            if (ok != MSG_OK)
            {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n" << std::endl;
            }
            
            dataLength = unpack<unsigned long int>(rpcIn);
            data = new char[dataLength];

            for (unsigned long int i = 0; i < dataLength; i++)
            {
                data[i] = rpcIn[i];
            }
        };

        // WRITE FILE --------
        void writeFile(const std::string fileName, char *data, unsigned long int dataLength)
        {
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, opWriteFile);

            int tam = fileName.length() + 1;
            pack(rpcOut, tam);

            packv(rpcOut, fileName.c_str(), tam);

            pack(rpcOut, dataLength);

            packv(rpcOut, data, dataLength);

            sendMSG(serverConnection.serverId, rpcOut);

            recvMSG(serverConnection.serverId, rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn);

            if (ok != MSG_OK)
            {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n" << std::endl;
            }
        };

        void freeListedFiles(std::vector<std::string*> *fileList)
        {

            for (int i = 0; i < fileList->size(); i++)
            {
                delete fileList->at(i);
            }
            delete fileList;
        };

};