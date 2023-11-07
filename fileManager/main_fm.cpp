#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "filemanagerstub.h"  



int main(int argc, char** argv){
    // Crea una instancia del stub del FileManager para interactuar con el servidor de archivos
    FileManager_Stub filemanager("172.31.63.167");  

    // Ejemplo: Listar archivos en el servidor
    std::vector<std::string> files;
    FileManagerOp.listFiles(files);
    std::cout << "Lista de ficheros en el servidor:\n";
    for (const std::string& file : files) {
        std::cout << "Fichero: " << file << std::endl;
    }

    
    char* data;
    unsigned long int dataLength;
    FileManagerOp.readFile("archivo.txt", data, dataLength);
    std::cout << "Contenido del archivo:\n";
    std::cout.write(data, dataLength);

    // Ejemplo: Escribir un archivo en el servidor
    char fileData[] = "Contenido para escribir en el archivo.";
    FileManagerOp.writeFile("nuevo_archivo.txt", fileData, sizeof(fileData));

    

    // Liberando datos de fichero leído
    delete[] data;
    return 0;
}
