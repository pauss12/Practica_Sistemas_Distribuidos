#include <iostream>
#include "filemanagerstub.h"  // Reemplaza con el nombre correcto del archivo de encabezado del stub

int main(int argc, char** argv) {
    // Crea una instancia del stub del FileManager para interactuar con el servidor de archivos
    FileManager_Stub filemanager(0);  // Reemplaza el 0 con el ID de cliente adecuado

    // Ejemplo: Listar archivos en el servidor
    std::vector<std::string> files;
    FileManagerOp.listFiles(files);
    std::cout << "Lista de ficheros en el servidor:\n";
    for (const std::string& file : files) {
        std::cout << "Fichero: " << file << std::endl;
    }

    // Ejemplo: Leer un archivo del servidor
    char* data;
    unsigned long int dataLength;
    FileManagerOp.readFile("archivo.txt", data, dataLength);
    std::cout << "Contenido del archivo:\n";
    std::cout.write(data, dataLength);

    // Ejemplo: Escribir un archivo en el servidor
    char fileData[] = "Contenido para escribir en el archivo.";
    FileManagerOp.writeFile("nuevo_archivo.txt", fileData, sizeof(fileData));

    // No es necesario liberar la lista de ficheros en este caso

    // Liberando datos de fichero leído
    delete[] data;
    return 0;
}
