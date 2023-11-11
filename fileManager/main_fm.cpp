#include <iostream>
#include "fileManager_Stub.h"
/*
int main(int argc,char** argv)
{
    FileManager_Stub *fm = new FileManager_Stub("./dirprueba/");
    
    std::vector<string *> *vfiles = fm->listFiles();

    std::cout<<"Lista de ficheros en el directorio de prueba:\n";

    for(int i=0; i<vfiles->size(); i++)
    {
        std::cout << " --- Fichero: " << vfiles->at(i)->c_str() << endl;
    }
    std::cout << "Leyendo el primer fichero del directorio de prueba:\n";
 
    char* data=nullptr;
    unsigned long int fileLen=0;
    fm->readFile(&(*(vfiles->at(0)))[0], data, fileLen);

  
    std::cout << "Escribiendo el primer fichero del directorio de prueba:\n";
    fm->writeFile(&(*(vfiles->at(0)))[0], data, fileLen);

    std::cout << "Liberando lista de ficheros:\n";
    fm->freeListedFiles(vfiles);
    std::cout << "Liberando datos de fichero leido:\n";

    delete[] data;

    return 0;
}
*/
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

    //Variables que necesito
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

                //calcular el tamaño de data
                int fileLen = data_string.length() + 1;

                fm->writeFile(fileName, data, fileLen);

                std::cout << "Liberando lista de ficheros:\n";
                
                fm->freeListedFiles(vfiles);

            }break;

            case 3:
            {
                /*
                --------------------------------------------------------
                               // PROBARLO EN CASA
   
                // Pedir los datos al usuario por terminal
                std::cout << "Introduce el nombre del fichero: ";

                // Guardar el nombre del fichero en fileName hasta el espacio
                std::cin >> fileName;

                // Limpiar el buffer
                while (getchar() != '\n')
                    ;

                --------------------------------------------------------
                */

                //Pedir un archivo remoto al usuario y descargarlo al directorio local
                std::string fileName = "mundo.txt";
                data_string = "hola mundo";

                data = (char *)data_string.data();

                //calcular el tamaño de data
                int fileLen = data_string.length() + 1;

                fm->readFile(fileName, data, fileLen);

            }break;

            case 4:
            {
                printf("Saliendo...\n");
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
