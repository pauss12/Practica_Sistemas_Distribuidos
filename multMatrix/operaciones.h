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
inline void packv(std::vector<unsigned char> &packet,T* data, int dataSize){
	
	for(int i=0;i<dataSize;i++)
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
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			pack(packet, data[i * cols + j]);
		}
	}

}

//DESEMPAQUETAR ENTEROS --------------------------------------------------------------------------------
inline int unpackInt(std::vector<unsigned char> &packet) {

  // Determinar el tamaño del entero.
  int dataSize = sizeof(int);

  // Leer el entero del vector de bytes.
  int data = 0;
  for (int i = 0; i < dataSize; i++) {
    data |= packet[i] << (8 * (dataSize - i - 1));
  }

  return data;
}

//DESEMPAQUETAR UNA MATRIZ -----------------------------------------------------------------------------------
template<typename T>
inline void unpackMatrix(std::vector<unsigned char> &packet, T *data, int rows, int cols) {

	// Desempaquetar el número de filas y columnas de la matriz.
	int numRows = unpackInt(packet);
	int numCols = unpackInt(packet);

	// Desempaquetar los datos de la matriz.
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			data[i * cols + j] = unpackInt(packet);
		}
	}
}

//RECIBIR UNA CADENA ---------------------------------------------------------------------
void recv_cadena(int id, std::string &dato)
{
	
	// Quiero que solamente me envie el tamaño de la cadena y su contenido
	std::vector<unsigned char> rpcIn;
	std::vector<unsigned char> rpcOut;

	std::cout << "entra en el recibe cadena" << std::endl;

	recvMSG(id, rpcIn);

	std::cout << "Recibe el tamaño de la cadena y la cadena" << std::endl;

	// Recibir el tamaño de la cadena
	int tam = unpack<int>(rpcIn);

	std::cout << "Recibe el tamaño de la cadena y la desempaqueta" << std::endl;

	// Recibir la cadena
	dato.resize(tam);

	std::cout << "Redimensiona la cadena" << std::endl;

	unpackv(rpcIn, (char *)dato.data(), tam);

	std::cout << "Recibe la cadena y la desempaqueta" << std::endl;

	// Enviar OK (0: no okey, 1: Okey)
	pack(rpcOut, MSG_OK);

	std::cout << "Empaqueta el OK" << std::endl;

	sendMSG(id, rpcOut);

	std::cout << "Envia el OK" << std::endl;
}

// ENVIAR UNA CADENA  ------------------------------------------------------------------
void send_cadena(int id, const std::string &dato)
{
	//Quiero que me empaquete solamente el tamaño de la cadena y su contenido
	std::vector<unsigned char> rpcOut;
	std::vector<unsigned char> rpcIn;

	std::cout << "entra en el manda cadena" << std::endl;

	// Empaquetar el tamaño de la cadena
	int tam = dato.length() + 1;
	pack(rpcOut, tam);

	std::cout << "Empaqueta el tamaño de la cadena" << std::endl;

	// Empaquetar la cadena
	packv(rpcOut, dato.data(), tam);

	std::cout << "Empaqueta la cadena" << std::endl;

	// Enviar el tamaño de la cadena y la cadena
	sendMSG(id, rpcOut);

	std::cout << "Envia el tamaño de la cadena y la cadena" << std::endl;

	// Recibir OK (0: no okey, 1: Okey)
	recvMSG(id, rpcIn);

	std::cout << "Recibe el OK" << std::endl;

	if (rpcIn[0] != MSG_OK)
		std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
}