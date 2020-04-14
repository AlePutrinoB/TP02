/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void* a_enviar = malloc(*bytes + sizeof(int) * 2);
	int offset = 0;

	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, paquete->buffer->stream, *bytes);

	*bytes = paquete->buffer->size + sizeof(int) * 2;

	return a_enviar;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
		int mensaje_length = strlen(mensaje) + 1;

		t_buffer* buffer = malloc(sizeof(t_buffer));
		buffer->size = mensaje_length;
		void * stream = malloc(buffer -> size);
		memcpy(stream, mensaje, mensaje_length);
		buffer->stream = stream;

		t_paquete* paquete = malloc(sizeof(t_paquete));

		paquete->codigo_operacion = MENSAJE;
		paquete->buffer = buffer;

		int bytes = paquete->buffer->size;

		void* a_enviar = serializar_paquete(paquete, &bytes);

		send(socket_cliente, a_enviar, bytes, 0);

		free(a_enviar);
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
}

//TODO
char* recibir_mensaje(int socket_cliente)
{
	op_code codigo;
	recv(socket_cliente, &codigo, sizeof(int), MSG_WAITALL);
	int tamanio;
	recv(socket_cliente, &tamanio, sizeof(int), MSG_WAITALL);
	char* mensaje;
	recv(socket_cliente, mensaje, tamanio, MSG_WAITALL);

	return mensaje;
}

char* deserealizar_mensaje(t_buffer* buffer){
	char* mensaje = malloc(buffer->size);
	void* stream = buffer->stream;
	memcpy(mensaje, stream, buffer->size);

	return mensaje;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
