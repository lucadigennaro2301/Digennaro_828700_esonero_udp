#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/* portabilità*/
#if defined WIN32

    // Librerie
    #include <winsock2.h>
    #include <ws2tcpip.h>

    // funzione confronto stringhe
    #define strcasecmp _stricmp

#else

    // Librerie
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define closesocket close

#endif

/* Librerie standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

/* Costanti del protocollo */
#define SERVER_PORT 56700     // Porta di default
#define MAX_CITY_LEN 64       // Lunghezza massima città
#define BUFFER_SIZE 512       // Buffer

/* Codici di stato risposta */
#define STATUS_OK 0
#define STATUS_CITY_NOT_FOUND 1
#define STATUS_INVALID_REQUEST 2

/* Tipi di dati meteo */
#define TYPE_TEMPERATURE 't'
#define TYPE_HUMIDITY    'h'
#define TYPE_WIND        'w'
#define TYPE_PRESSURE    'p'

/* richiesta meteo */
typedef struct {
    char type;                       // Tipo richiesta
    char city[MAX_CITY_LEN];         // Nome città
} weather_request_t;

/* risposta meteo */
typedef struct {
    unsigned int status;             // Stato risposta
    char type;                       // Tipo dato
    float value;                     // Valore meteo
} weather_response_t;

static inline void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

/*funzioni generazione dati meteo */
float get_temperature(void);    // Temperatura
float get_humidity(void);       // Umidità
float get_wind(void);           // Vento
float get_pressure(void);       // Pressione

/* Verifica città */
int is_supported_city(const char* city);

#endif
