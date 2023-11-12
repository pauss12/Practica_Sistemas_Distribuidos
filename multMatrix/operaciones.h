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

//EMPAQUETAR UN ARRAY ------------------------------------------------------------------------------
template<typename T>
inline void packv(std::vector<unsigned char> &packet, T* data, int dataSize){
	
	for(int i=0; i < dataSize; i++)
		pack(packet, data[i]);
}

template <typename T>
inline T unpack(std::vector<unsigned char> &packet)
{
	T data;
	int dataSize = sizeof(T);
	int packetSize = packet.size();
	T *ptr = (T *)packet.data();
	data = ptr[0];

	for (int i = dataSize; i < packetSize; i++)
	{
		packet[i - dataSize] = packet[i];
	}

	packet.resize(packetSize - dataSize);
	return data;
}

template <typename T>
inline void unpackv(std::vector<unsigned char> &packet, T *data, int dataSize)
{
	for (int i = 0; i < dataSize; i++)
		data[i] = unpack<T>(packet);
}

//EMPAQUETAR LA MATRIZ ----------------------------------------------------------------------------
template<typename T>
inline void packMatrix(std::vector<unsigned char> &packet, T *data, int rows, int cols) {

	// Empaquetar el número de filas y columnas de la matriz.
	pack(packet, rows);
	pack(packet, cols);

	// Empaquetar los datos de la matriz.
	packv(packet, data, rows * cols);
}

// ENVIAR UNA CADENA  ------------------------------------------------------------------
void send_cadena(int id, const std::string &dato, operacionesEnum op)
{
	//Quiero que me empaquete solamente el tamaño de la cadena y su contenido
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	//Empaquetar la operacion
	pack(rpcOut, op);

	// Empaquetar el tamaño de la cadena
	int tam = dato.length() + 1;
	pack(rpcOut, tam);

	// Empaquetar la cadena
	packv(rpcOut, dato.data(), tam);

	// Enviar el tamaño de la cadena y la cadena
	sendMSG(id, rpcOut);
}