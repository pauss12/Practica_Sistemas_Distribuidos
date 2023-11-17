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
    std::string data_string;
    char *data = nullptr;
    unsigned long int fileLen = 0;

    // Para ver la carpeta del server
    FileManager_Stub *fm = new FileManager_Stub("./dirprueba/");

    // Para ver la carpeta local
    FileManager *fm_local = new FileManager("./dirprueba/");

    do {

        mostrarComandos();
        scanf("%d", &opcion);
        while (getchar() != '\n');

        switch (opcion)
        {
            case 1:
            {   
                //Listar archivos del directorio remoto
                std::vector<std::string *> *vfiles = fm->listFiles();

                for (int i = 0; i < vfiles->size(); i++)
                {
                    std::cout << " --- Fichero: " << vfiles->at(i)->c_str() << endl;
                }

                fm->freeListedFiles(vfiles);

            }break;

            case 2:
            {

                std::vector<std::string *> *vfiles = fm->listFiles();

                std::string fichero;

                // darle las opciones
                std::vector<std::string *> *vfiles_local = fm_local->listFiles();

                for (int i = 0; i < vfiles_local->size(); i++)
                {
                    std::cout << " --- Fichero: " << vfiles_local->at(i)->c_str() << endl;
                }

                // Pedir el nombre de un archivo al usuario y subirlo al directorio remoto
                std::cout << "\nIntroduce el nombre del fichero: " << std::endl;
                std::cin >> fichero;

                fm_local->readFile((char *)fichero.data(), data, fileLen);

                std::cout << "Pasando el archivo [ " << fichero.data() << "] al servidor "
                          << "\n"
                          << std::endl;
                fm->writeFile((char *)fichero.data(), data, fileLen);

                fm->freeListedFiles(vfiles);
                fm_local->freeListedFiles(vfiles_local);
            }
            break;

            case 3:
            {

                // Pedir el nombre de un archivo al usuario y descargarlo del directorio remoto
                std::string fileName;

                std::cout << "\nIntroduce el nombre del fichero: " << std::endl;
                std::cin >> fileName;
                
                fm->readFile((char *)fileName.data(), data, fileLen);

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
