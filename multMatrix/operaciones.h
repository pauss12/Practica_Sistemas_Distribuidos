#pragma once
#include <vector>
#include <cstring>


typedef enum{
	opMultiplicar=1,
	opCrearIdentidad=2,
	opCrearRandom=3,
	opEscribirMatriz=4,
	opLeerMatriz=5
	
}operacionesEnum;

typedef struct __attribute__((packed))
{
	operacionesEnum tipoOperacion; 
	union{
		
		struct {
			matrix_t matrizA;
			matrix_t matrizB;		
		}Multiplicar;
		
		struct {	
			int filas;
			int columnas;		
		}crearIdentidad;
		
		struct {
			int filas;
			int columnas;
			int rangoMax;
			int rangoMin;
		}crearRandom;
		
		struct {
			char *rutaArchivo;
			int tam;
		}leerMatriz;
		
		struct {
			char *rutaArchivo;
			int tam;
			matrix_t A;
		}escribirMatriz;
	};
}operacion_t;

//EMPAQUETAR UNA CADENA (SU CONTENIDO)
template<typename T>
inline void packv(std::vector<unsigned char> &packet,T* data, int dataSize){
	
	for(int i=0;i<dataSize;i++)
		pack(packet, data[i]);
}

inline void empaquetaOperacion(std::vector<unsigned char> &packet,operacion_t  op)
{
	unsigned char tipoOp=op.tipoOperacion;
	pack(packet, tipoOp);
	
	switch(op.tipoOperacion)
	   {
		case opMultiplicar:
		{
			pack(packet,op.sumaInt.operando1);
			pack(packet,op.sumaInt.operando2);
		}break;
		 
		case opCrearIdentidad:
		{

		}break;
		 
		case opCrearRandom:
		{
			
		}break;
		 
		case opEscribirMatriz:
		{
			pack(packet, op.tipoOperacion.tam);
			packv(packet, op.tipoOperacion.data(), tam);
		}break;
		 
		case opLeerMatriz:
		{
			pack(packet, op.tipoOperacion.tam);
			packv(packet, op.tipoOperacion.data(), tam);
		}break;
		
		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	};
}


















