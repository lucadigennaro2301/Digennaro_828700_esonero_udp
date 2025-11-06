/*
 * main.c
 *
 * Server TCP - Template per esercitazione Reti di Calcolatori
 *
 * Questo file contiene il codice boilerplate per un server TCP
 * portabile su Windows, Linux e MacOS.
 */

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define NO_ERROR 0

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

	// TODO: Implementare la logica del server

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	int my_socket;

	// TODO: Creare il socket
	// my_socket = socket(...);

	// TODO: Configurare l'indirizzo del server
	// struct sockaddr_in server_addr;
	// server_addr.sin_family = AF_INET;
	// server_addr.sin_port = htons(SERVER_PORT);
	// server_addr.sin_addr.s_addr = INADDR_ANY;

	// TODO: Bind del socket
	// bind(...);

	// TODO: Mettere il socket in ascolto
	// listen(...);

	// TODO: Implementare il ciclo di accettazione connessioni
	// while (1) {
	//     int client_socket = accept(...);
	//     // Gestire la comunicazione con il client
	//     closesocket(client_socket);
	// }

	printf("Server terminato.\n");

	closesocket(my_socket);
	clearwinsock();
	return 0;
} // main end
