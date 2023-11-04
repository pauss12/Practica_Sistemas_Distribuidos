#pragma once
#include <vector>
#include <cstring>

#define MSG_OK 1
#define MSG_NOK 0

typedef enum{
	opConstructor=1,
	opDestructor=2,
	opMultiplicarMatrices=3,
	opCrearIdentidad=4,
	opCrearRandom=5,
	opEscribirMatriz=6,
	opLeerMatriz=7
	
}operacionesEnum;

template<typename T>
inline void pack(std::vector<unsigned char> &packet, T data) {

	int size = packet.size();
	unsigned char *ptr = (unsigned char*)&data;
	packet.resize(size + sizeof(T));
	std::copy(ptr, ptr + sizeof(T), packet.begin() + size);
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

/*
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
			matrix_t A;
		}leerMatriz;
		
		struct {
			char *rutaArchivo;
			int tam;
		}escribirMatriz;
	};
}operacion_t;



//EMPAQUETAR UN ARRAY ------------------------------------------------------------------------------
template<typename T>
inline void packv(std::vector<unsigned char> &packet,T* data, int dataSize){
	
	for(int i=0;i<dataSize;i++)
		pack(packet, data[i]);
}

//EMPAQUETAR LA MATRIZ ----------------------------------------------------------------------------
template<typename T>
inline void packMatrix(std::vector<unsigned char> &packet, T *data, int rows, int cols) {

	// Empaquetar el número de filas y columnas de la matriz.
	pack(packet, rows);
	pack(packet, cols);

	// Empaquetar los datos de la matriz.
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			pack(packet, data[i * cols + j]);
		}
	}

}

//EMPAQUETAR LA OPERACION QUE VA A REALIZAR ----------------------------------------------------------
inline void empaquetaOperacion(std::vector<unsigned char> &packet,operacion_t  op)
{
	unsigned char tipoOp=op.tipoOperacion;
	pack(packet, tipoOp);
	
	switch(op.tipoOperacion)
	   {
		case opMultiplicar:
		{
			packMatrix(op.tipoOperacion.matrizA);
			packMatrix(op.tipoOperacion.matrizB);
		}break;
		 
		case opCrearIdentidad:
		{
			pack(&packet, op.tipoOperacion.filas);
			pack(&packet, op.tipoOperacion.columnas);
		}break;
		 
		case opCrearRandom:
		{
			pack(&packet, op.tipoOperacion.filas);
			pack(&packet, op.tipoOperacion.columnas);
			pack(&packet, op.tipoOperacion.rangoMax);
			pack(&packet, op.tipoOperacion.rangoMin);
			
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
			
			//¿Hay que empaquetar la matriz?
		}break;
		
		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	};
}
*/














