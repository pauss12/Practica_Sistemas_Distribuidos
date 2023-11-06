#include <iostream>
#include <vector>
#include <string>
#include "utils.h"
#include "operacionesFiles.h"

enum FileManagerOp {
    OpListFiles = 8,
    OpReadFile = 9,
    OpWriteFile = 10
};

class FileManager_Stub {
private:
    connection_t serverConnection;

    void sendStringOp(int serverId, const std::string& dato) {
        std::vector<unsigned char> rpcOut;
        std::vector<unsigned char> rpcIn;

        // En lugar de especificar la operación 'op', utilizamos la operación correspondiente de FileManager.
        pack(rpcOut, OpWriteFile);

        int tam = dato.length() + 1;
        pack(rpcOut, tam);
        packv(rpcOut, dato.c_str(), tam);

        sendMSG(serverId, rpcOut);
        recvMSG(serverId, rpcIn);

        if (rpcIn[0] != MSG_OK) {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
    }

    void recvStringOp(int serverId, std::string& dato) {
        std::vector<unsigned char> rpcOut;
        std::vector<unsigned char> rpcIn;

        // En lugar de especificar la operación 'op', utilizamos la operación correspondiente de FileManager.
        pack(rpcOut, OpReadFile);
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

public:
    FileManager_Stub(const std::string& serverIP, int serverPort) {
        serverConnection = initClient(serverIP, serverPort);
    }

    ~FileManager_Stub() {
        closeConnection(serverConnection.serverId);
    }

    std::vector<std::string>* listFiles() {
        std::vector<unsigned char> rpcOut;
        std::vector<unsigned char> rpcIn;

        pack(rpcOut, OpListFiles);

        sendMSG(serverConnection.serverId, rpcOut);
        recvMSG(serverConnection.serverId, rpcIn);

        if (rpcIn[0] != MSG_OK) {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
            return nullptr;
        } else {
            int numFiles = unpack<int>(rpcIn);
            std::vector<std::string>* fileList = new std::vector<std::string>();
            for (int i = 0; i < numFiles; i++) {
                std::string fileName;
                recvStringOp(serverConnection.serverId, fileName);
                fileList->push_back(fileName);
            }
            return fileList;
        }
    }

    void readFile(const std::string& fileName, char* &data, unsigned long int &dataLength) {
        std::vector<unsigned char> rpcOut;
        std::vector<unsigned char> rpcIn;

        pack(rpcOut, OpReadFile);
        sendStringOp(serverConnection.serverId, fileName);

        // Receive data
        recvMSG(serverConnection.serverId, rpcIn);

        if (rpcIn[0] != MSG_OK) {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        } else {
            dataLength = unpack<unsigned long int>(rpcIn);
            data = new char[dataLength];
            // Receive file content
            recvMSG(serverConnection.serverId, rpcIn);
            for (unsigned long int i = 0; i < dataLength; i++) {
                data[i] = rpcIn[i];
            }
        }
    }

    void writeFile(const std::string& fileName, const char* data, unsigned long int dataLength) {
        std::vector<unsigned char> rpcOut;

        pack(rpcOut, OpWriteFile);
        sendStringOp(serverConnection.serverId, fileName);

        pack(rpcOut, dataLength);
        for (unsigned long int i = 0; i < dataLength; i++) {
            rpcOut.push_back(data[i]);
        }

        sendMSG(serverConnection.serverId, rpcOut);
    }

    void freeListedFiles(std::vector<std::string>* fileList) {
    // Liberar la memoria de cada cadena individual en la lista.
    for (auto fileName : *fileList) {
        // No necesitas usar delete para las cadenas, ya que son objetos de la STL y se administran automáticamente.
    }

    // Luego, elimina la lista en sí.
    delete fileList;
}

};
