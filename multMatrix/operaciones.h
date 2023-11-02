#pragma once
#include <vector>
#include <cstring>
#include <iostream>

using namespace std;
typedef enum MatrixOperation {
    // Definir las operaciones aquí
    ConstructorOp,
    DestructorOp,
    ReadMatrixOp,
    MultMatricesOp,
    WriteMatrixOp,
    CreateIdentityOp,
    CreateRandMatrixOp,
    // ... otras operaciones ...
}MatrixOperation;


#define MSG_OK 1
#define MSG_NOK 0

typedef enum matrizOp{
	constructorOp=1,
	destructorOp=2,
	setNombreOp=3,
	setDniOp=4,
	saveOp=5,
	loadOp=6,
	getNombreOp=7,
	getDniOp=8
	
}personaOp;




typedef struct __attribute__((packed))
{
	operacionesEnum tipoOperacion; //tipo operacion
	union{
		struct {//operandos entrada
			int operando1;
			int operando2;
		}sumaInt;
		struct {
			float operando1;
			float operando2;
		}sumaFloat;
		struct {
			double operando1;
			double operando2;
		}sumaDoubles;
		struct {
			int operando1[3];
			int operando2[3];
		}sumaVector3Int;
	};
}operacion_t;

typedef struct resultado_t{
	operacionesEnum tipoOperacion; //tipo operacion
	union{
		struct {//resultados salida
			int res;
		}sumaInt;
		struct {
			float res;
		}sumaFloat;
		struct {
			double res;
		}sumaDoubles;
		struct {
			int res[3];
		}sumaVector3Int;
	};	
}resultado_t;


template<typename T>
inline void pack(std::vector<unsigned char> &packet,T data){
	
	int size=packet.size();
	unsigned char *ptr=(unsigned char*)&data;
	packet.resize(size+sizeof(T));
	std::copy(ptr,ptr+sizeof(T),packet.begin()+size);
	
}


template<typename T>
inline void packv(std::vector<unsigned char> &packet,T* data,int dataSize)
{
	for(int i=0;i<dataSize;i++)
			 	pack(packet,data[i]);
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



template<typename T>
inline void unpackv(std::vector<unsigned char> &packet,T* data,int dataSize)
{
	for(int i=0;i<dataSize;i++)
		data[i]=unpack<T>(packet);
}




inline void empaquetaOperacion( std::vector<unsigned char> &packet,operacion_t  op)
{
	//primer byte: tipo operación  
	//             máx 	256 tipos de operación
	//si tipo op es sumaInt o sumaFloat , 
			//siguientes 8 bytes, dos operandos
	//si tipo op es suma double , 
			//siguientes 16 bytes, dos operandos 
	//si tipo op es suma vector i , 
			//siguientes 12 bytes, 3  integers
			//siguientes 12 bytes, 3  integers
	unsigned char tipoOp=op.tipoOperacion;
	pack(packet, tipoOp);
	
	
	switch(op.tipoOperacion)
	   {
		 case opSumaInt:
		 {

				pack(packet,op.sumaInt.operando1);
				pack(packet,op.sumaInt.operando2);
		 }break;
		
		 case opSumaFloat:
		 {
			 	pack(packet,op.sumaFloat.operando1);
				pack(packet,op.sumaFloat.operando2);
			 
		 }break;
		 
		 case opSumaDoubles:
		 {
			 	pack(packet,op.sumaDoubles.operando1);
				pack(packet,op.sumaDoubles.operando2);
			 
		 }break;
		 case opSumaVector3Int:
		 {
			 for(int i=0;i<3;i++)
			 	pack(packet,op.sumaVector3Int.operando1[i]);
			 for(int i=0;i<3;i++)
				pack(packet,op.sumaVector3Int.operando2[i]);
		 }break;
		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	   };
	
}


inline operacion_t desempaquetaOperacion(std::vector<unsigned char> &packet)
{
	//primer byte: tipo operación  
	//             máx 	256 tipos de operación
	//si tipo op es sumaInt o sumaFloat , 
			//siguientes 8 bytes, dos operandos
	//si tipo op es suma double , 
			//siguientes 16 bytes, dos operandos 
	//si tipo op es suma vector i , 
			//siguientes 12 bytes, 3  integers
			//siguientes 12 bytes, 3  integers
	operacion_t  op;
	unsigned char tipoOp=unpack<unsigned char>(packet);
	op.tipoOperacion=(operacionesEnum)tipoOp;
	
	switch(op.tipoOperacion)
	   {
		 case opSumaInt:
		 {

				op.sumaInt.operando1=unpack<int>(packet);
				op.sumaInt.operando2=unpack<int>(packet);

		 }break;
		
		 case opSumaFloat:
		 {

				op.sumaFloat.operando1=unpack<float>(packet);
				op.sumaFloat.operando2=unpack<float>(packet);
			 
		 }break;
		 
		 case opSumaDoubles:
		 {
				op.sumaDoubles.operando1=unpack<double>(packet);
				op.sumaDoubles.operando2=unpack<double>(packet);	 
		 }break;
		 case opSumaVector3Int:
		 {
			 for(int i=0;i<3;i++)
				op.sumaVector3Int.operando1[i]=unpack<int>(packet);
			 for(int i=0;i<3;i++)
				op.sumaVector3Int.operando2[i]=unpack<int>(packet);
		 }break;
		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	   };
	return op;
}

















