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
    int clientId;
    FileManager* fileManager = nullptr;

public:s
    FileManager_Imp(int clientId) : clientId(clientId), fileManager(nullptr) {
        // Constructor, inicializa lo que necesites.
    }

    bool connectionClosed() {
        return fileManager == nullptr;
    }

    void recibeOp() {
        std::vector<unsigned char> rpcIn;
        std::vector<unsigned char> rpcOut;

        // Recibe operación
        recvMSG(clientId, rpcIn);

       operacionesEnum operacion=unpack<operacionesEnum>(rpcIn);

        // Ejecuta la operación correspondiente y genera una respuesta.
        switch (operacion) {
            case opConstructor:
                if (fileManager == nullptr) {
                    // Si el FileManager no está inicializado, crea una nueva instancia.
                    fileManager = new FileManager();
                    pack(rpcOut, (unsigned char)MSG_OK);
                } else {
                    // Ya existe una instancia, envía un error.
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
                break;

            case opDestructor:
                if (fileManager != nullptr) {
                    // Si el FileManager está inicializado, elimina la instancia.
                    delete fileManager;
                    fileManager = nullptr;
                    pack(rpcOut, (unsigned char)MSG_OK);
                } else {
                    // No hay una instancia para destruir, envía un error.
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
                break;

          /*  case opListFiles:
                if (fileManager != nullptr) {
                    // Si el FileManager está inicializado, realiza la operación "listFiles".
                    std::vector<std::string*>* fileList = fileManager->listFiles();

                    if (fileList != nullptr) {
                        // Si la operación fue exitosa, empaqueta los resultados y envía un OK.
                        pack(rpcOut, (unsigned char)MSG_OK);
                        // Aquí debes implementar la lógica para empaquetar los nombres de archivos y sus tamaños en rpcOut.
                    } else {
                        // La operación "listFiles" falló, envía un error.
                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }
                } else {
                    // No hay una instancia de FileManager para realizar la operación, envía un error.
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }
                break;
             case opListFiles:
                    if (fileManager != nullptr) {
                        // Si el FileManager está inicializado, realiza la operación "listFiles".
                        std::vector<std::string*>* fileList = fileManager->listFiles();

                        if (fileList != nullptr) {
                            // Si la operación fue exitosa, empaqueta los resultados y envía un OK.
                            pack(rpcOut, (unsigned char)MSG_OK);
                            // Aquí debes implementar la lógica para empaquetar los nombres de archivos y sus tamaños en rpcOut.
                            for (const std::string& filename : *fileList) {
                                // Agregar el nombre del archivo al paquete
                                pack(rpcOut, filename);

                            
                                std::string fileSize = obtenerTamanoDelArchivo(filename);
                                pack(rpcOut, fileSize);
                            }
                        } else {
                            // La operación "listFiles" falló, envía un error.
                            pack(rpcOut, (unsigned char)MSG_NOK);
                        }
                    } else {
                        // No hay una instancia de FileManager para realizar la operación, envía un error.
                        pack(rpcOut, (unsigned char)MSG_NOK);
                    }
                    break;

            // Implementar el resto de operaciones (opReadFile, opWriteFile) de manera similar.

            default:
                // Operación no definida, envía un error.
                std::cout<<"Error: función no definida\n";
                pack(rpcOut, (unsigned char)MSG_NOK);
                break;
        }

        // Envía la respuesta al cliente.
        sendMSG(clientId, rpcOut);
    }
};
