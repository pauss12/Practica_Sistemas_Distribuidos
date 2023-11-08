
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include <string>
#include "filemanagerstub.h"
#include "operacionesFiles.h"

int main(int argc, char** argv) {
    // Crear una instancia de FileManager con el directorio de trabajo "./dirprueba/"
    FileManager fm= new FileManager();
    fm.setWorkingDirectory("./dirprueba/");

    // Listar archivos en el directorio
    std::vector<std::string> vfiles = fm.listFiles();

    std::cout << "Lista de ficheros en el directorio de prueba:\n";
    for (const std::string& file : vfiles) {
        std::cout << "Fichero: " << file << std::endl;
    }

    if (!vfiles.empty()) {
        // Leer el primer archivo en la lista
        std::string fileName = vfiles[0];
        std::cout << "Leyendo el primer fichero del directorio de prueba: " << fileName << std::endl;

        char* data = nullptr;
        unsigned long int fileLen = 0;
        if (fm.readFile(fileName, data, fileLen)) {
            std::cout << "Contenido del archivo:\n";
            std::cout.write(data, fileLen);

            // Escribir el mismo archivo de vuelta al directorio
            std::cout << "Escribiendo el primer fichero del directorio de prueba: " << fileName << std::endl;
            if (fm.writeFile(fileName, data, fileLen)) {
                std::cout << "Escritura exitosa." << std::endl;
            } else {
                std::cout << "Error al escribir el archivo." << std::endl;
            }

            delete[] data;
        } else {
            std::cout << "Error al leer el archivo: " << fileName << std::endl;
        }
    } else {
        std::cout << "No hay archivos para leer en el directorio." << std::endl;
    }

    return 0;
}