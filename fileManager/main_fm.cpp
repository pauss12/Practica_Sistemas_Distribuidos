#include <iostream>
#include "utils.h"
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
        while (getchar() != '\n');

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
                std::string aux;

                std::cout << "\nIntroduce el nombre del fichero: " << std::endl;

                // Coger la linea entera
                std::getline(std::cin, aux);

                int tam = aux.size() + 1;

                char *fileName = new char[tam];

                for (int i = 0; i < tam; i++)
                {
                    fileName[i] = aux[i];
                }

                fileName[tam] = '\0';

                std::cout << "Introduce el contenido del fichero: ";
                std::getline(std::cin, data_string);

                data = (char *)data_string.data();

                int fileLen = data_string.length() + 1;

                fm->writeFile(fileName, data, fileLen);
                
                fm->freeListedFiles(vfiles);

            }break;

            case 3:
            {

                // Pedir el nombre de un archivo al usuario y descargarlo del directorio remoto
                std::string aux;

                std::cout << "\nIntroduce el nombre del fichero: " << std::endl;

                //Coger la linea entera
                std::getline(std::cin, aux);

                int tam = aux.size() + 1;

                char *fileName = new char[tam];

                for (int i = 0; i < tam; i++)
                {
                    fileName[i] = aux[i];
                }

                fileName[tam] = '\0';

                data = nullptr;
                fileLen = 0;

                int fileLen = data_string.length() + 1;

                fm->readFile(fileName, data, fileLen);
            }break;

            case 4:
            {
                printf("Saliendo...\n");
                delete fm;
                break;

            }break;

            default:
            {
                printf("Comando no reconocido\n");
            }break;

        };
    
    } while (opcion != 4 || (opcion < 1 || opcion > 4));

    return 0;
}
