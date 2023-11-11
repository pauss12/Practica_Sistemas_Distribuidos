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

            std::cout << "Enviando destructor\n" << std::endl;

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

        void readFile(const std::string &fileName, char *data, unsigned long int dataLength)
        {
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, opReadFile);

            int tam = fileName.length() + 1;
            pack(rpcOut, tam);
            packv(rpcOut, fileName.c_str(), tam);

            sendMSG(serverConnection.serverId, rpcOut);

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

        void writeFile(const std::string &fileName, char *data, unsigned long int dataLength)
        {
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, opWriteFile);

            //Empaquetar el nombre del fichero

            //Empaquetar el dataLength

            // Empaquetar el data

            //Mandarlo al server

        };

        void freeListedFiles(std::vector<std::string*> *fileList)
        {

            for (int i = 0; i < fileList->size(); i++)
            {
                delete fileList->at(i);
            }
            delete fileList;
        };

        /*
            ANTIGUA FUNCION, BASARSE EN ELLA
            --------------------------------------------------------------------
            unsigned long int tam = fileName.length() + 1;

            std::cout << "Tamaño del nombre del fichero: " << tam << "\n" << std::endl;

            pack(rpcOut, tam);

            std::cout << "Empaquetado el tamaño del nombre del fichero\n" << std::endl;

            packv(rpcOut, fileName.data(), tam);

            std::cout << "Empaquetado el nombre del fichero\n" << std::endl;

            sendMSG(serverConnection.serverId, rpcOut);

            std::cout << "Enviado el mensaje\n" << std::endl;

            recvMSG(serverConnection.serverId, rpcIn);

            std::cout << "Recibido el mensaje\n" << std::endl;

            unsigned char ok = unpack<unsigned char>(rpcIn);

            std::cout << "Desempaquetado el mensaje\n" << std::endl;

            if (ok != MSG_OK)
            {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n" << std::endl;
            }

            //Desempaquetar el tamaño de los datos
            dataLength = unpack<unsigned long int>(rpcIn);

            std::cout << "Desempaquetado el tamaño de los datos\n" << std::endl;

            //Desempaquetar los datos
            data = new char[dataLength];

            std::cout << "Reservado el tamaño de los datos\n" << std::endl;

            for (unsigned long int i = 0; i < dataLength; i++)
            {
                data[i] = rpcIn[i];
            }

            std::cout << "Desempaquetado los datos\n" << std::endl;

            --------------------------------------------------------------------

        */
};