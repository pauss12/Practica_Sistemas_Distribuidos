#include <iostream>
#include "fileManager_stub.h"

void mostrarComandos()
{
    printf("\nComandos:-------\n");
    printf("1. ls\n");
    printf("2. Upload \n");
    printf("3. Download\n");
    printf("4. Salir\n");
}

//MAIN MEJORADO
int main(void)
{
    int opcion = 0;
    FileManager_Stub *fm = new FileManager_Stub("./dirprueba/");

    std::vector<string *> *vfiles = nullptr;

    std::string data_string;

    char *data = nullptr;

    unsigned long int fileLen = 0;

    do {

        mostrarComandos();
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
            {   
                //Listar archivos del directorio remoto
                vfiles = fm->listFiles();

                for (int i = 0; i < vfiles->size(); i++)
                {
                    std::cout << " --- Fichero: " << vfiles->at(i)->c_str() << endl;
                }

            }break;

            case 2:
            {
                //Pedir el nombre de un archivo al usuario y subirlo al directorio remoto
                std::string fileName;

                //Pedir los datos al usuario por terminal
                std::cout << "Introduce el nombre del fichero: ";
                
                //Guardar el nombre del fichero en fileName hasta el espacio
                std::cin >> fileName;
                
                //Limpiar el buffer
                while(getchar() != '\n');

                std::cout << "Introduce el contenido del fichero: ";
                std::getline(std::cin, data_string);

                data = (char *)data_string.data();

                int fileLen = data_string.length() + 1;

                fm->writeFile(fileName, data, fileLen);
                
                fm->freeListedFiles(vfiles);

            }break;

            case 3:
            {

                std::string fileName;
                //Pedir un archivo remoto al usuario y descargarlo al directorio local

                std::cout << "\nIntroduce el nombre del fichero: " << std::endl;

                std::cin >> fileName;

                while (getchar() != '\n');

                data = nullptr;
                fileLen = 0;

                int fileLen = data_string.length() + 1;

                fm->readFile(fileName, data, fileLen);

            }break;

            case 4:
            {
                printf("Saliendo...\n");
                
                //Llamar al destructor del cliente
                fm->~FileManager_Stub();

                break;

            }break;

            default:
            {
                printf("Comando no reconocido\n");
            }break;

        };
    
    } while (opcion != 4);

    return 0;
}
