#pragma once
#include <vector>
#include <cstring>
#include "utils.h"
#define MSG_OK 1
#define MSG_NOK 0

typedef enum FileManagerOp{
    opConstructor =1,
    opDestructor=2,
    opListFiles = 3,
    opReadFile = 4,
    opWriteFile = 5

}FileManagerOp;


template<typename T>
inline void pack(std::vector<unsigned char> &packet,T data){
	
	int size=packet.size();
	unsigned char *ptr=(unsigned char*)&data;
	packet.resize(size+sizeof(T));
	std::copy(ptr,ptr+sizeof(T),packet.begin()+size);
	
}


template<typename T>
inline void packv(std::vector<unsigned char> &packet,T* data,int dataSize)
{
	for(int i=0;i<dataSize;i++)
			 	pack(packet,data[i]);
}


template<typename T>
inline T unpack(std::vector<unsigned char> &packet){	
	T data;
	int dataSize=sizeof(T);
	int packetSize=packet.size();
	T* ptr=(T*)packet.data();
	data=ptr[0];
	
	for(int i=dataSize;i<packetSize;i++)
	{
		packet[i-dataSize]=packet[i];	
	}
	
	packet.resize(packetSize-dataSize);
	return data;
}



template<typename T>
inline void unpackv(std::vector<unsigned char> &packet,T* data,int dataSize)
{
	for(int i=0;i<dataSize;i++)
		data[i]=unpack<T>(packet);
}


// Enviar una solicitud para listar archivos al servidor del "FileManager."
void sendListFiles(int id, FileManagerOp op) {
    
    std::vector<unsigned char> rpcOut;
    pack(rpcOut, opListFiles);
    sendMSG(id, rpcOut);
}

// Recibir la respuesta del servidor del "FileManager" con una lista de archivos.
void receiveListFiles(int id, std::vector<std::string> &fileList, FileManagerOp op) {
    std::vector<unsigned char> rpcIn;
    recvMSG(id, rpcIn);

    if (rpcIn[0] != MSG_OK) {
        std::cout << "Error al recibir la lista de archivos." << std::endl;
    } else {
        // Desempaquetar la lista de archivos y agregarlos al vector 'fileList.'
        int numFiles = unpack<int>(rpcIn);
        fileList.clear();
        for (int i = 0; i < numFiles; i++) {
            int fileNameLen = unpack<int>(rpcIn);
            char fileName[fileNameLen];
            unpackv(rpcIn, fileName, fileNameLen);
            fileList.push_back(fileName);
        }
    }
}

// Enviar una solicitud para leer un archivo al servidor del "FileManager."
void sendReadFile(int id, const std::string &fileName, FileManagerOp op) {
    std::vector<unsigned char> rpcOut;
    pack(rpcOut, opReadFile);
    int fileNameLen = fileName.length() + 1;
    pack(rpcOut, fileNameLen);
    packv(rpcOut, fileName.c_str(), fileNameLen);
    sendMSG(id, rpcOut);
}

// Recibir el contenido de un archivo desde el servidor del "FileManager."
void receiveReadFile(int id, char *data, unsigned long int &dataLength, FileManagerOp op) {
    std::vector<unsigned char> rpcIn;
    recvMSG(id, rpcIn);

    if (rpcIn[0] != MSG_OK) {
        std::cout << "Error al recibir el archivo." << std::endl;
    } else {
        // Desempaquetar el contenido del archivo.
        dataLength = unpack<unsigned long int>(rpcIn);
        data = new char[dataLength];
        unpackv(rpcIn, data, dataLength);
    }
}

// Enviar una solicitud para escribir un archivo al servidor del "FileManager."
void sendWriteFile(int id, const std::string &fileName, const char *data, unsigned long int dataLength, FileManagerOp op) {
    std::vector<unsigned char> rpcOut;
    pack(rpcOut, opWriteFile);
    int fileNameLen = fileName.length() + 1;
    pack(rpcOut, fileNameLen);
    packv(rpcOut, fileName.c_str(), fileNameLen);
    pack(rpcOut, dataLength);
    packv(rpcOut, data, dataLength);
    sendMSG(id, rpcOut);
}

// Recibir la confirmación del servidor del "FileManager" después de escribir un archivo.
void receiveWriteFileConfirmation(int id, FileManagerOp op) {
    std::vector<unsigned char> rpcIn;
    recvMSG(id, rpcIn);

    if (rpcIn[0] != MSG_OK) {
        std::cout << "Error al escribir el archivo en el servidor." << std::endl;
    } else {
        std::cout << "Archivo escrito con éxito en el servidor." << std::endl;
    }
}

