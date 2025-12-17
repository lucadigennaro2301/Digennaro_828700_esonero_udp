#include "protocol.h"     // Definizioni del protocollo e costanti
#include <stdint.h>       // Tipi interi a dimensione fissa

int main(int argc, char* argv[]) {

    int server_port = SERVER_PORT;     // Porta del server
    char* server_host = "localhost";   // Host del server
    char type = 0;                     // Tipo di richiesta (t, h, w, p)
    char city[MAX_CITY_LEN] = "";      // Nome della città

    for (int i = 1; i < argc; i++) {

        // Opzione server
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
            server_host = argv[++i];

        // Opzione porta
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
            server_port = atoi(argv[++i]);

        // Opzione richiesta "type city"
        else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {

            char full_req[256];
            strncpy(full_req, argv[i + 1], 255);   // Copia sicura

            // Controllo caratteri di tabulazione
            if (strchr(full_req, '\t')) {
                printf("Errore: la richiesta non ammette tabulazioni.\n");
                return -1;
            }

            // Il primo token deve essere un solo carattere
            char* first_space = strchr(full_req, ' ');
            if (first_space == NULL || (first_space - full_req) != 1) {
                printf("Errore: il primo token deve essere un singolo carattere.\n");
                return -1;
            }

            type = full_req[0];                 // Estrae il tipo
            char* city_part = first_space + 1;

            // Salta eventuali spazi multipli
            while (*city_part == ' ')
                city_part++;

            // Controllo lunghezza città
            if (strlen(city_part) >= MAX_CITY_LEN) {
                printf("Errore: nome città troppo lungo.\n");
                return -1;
            }

            strcpy(city, city_part);             // Copia il nome città
        }
    }

    // Controllo parametri obbligatori
    if (!type || strlen(city) == 0) {
        printf("Uso: ./client-project [-s server] [-p port] -r \"type city\"\n");
        return -1;
    }

#if defined WIN32
    // Inizializzazione Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        return -1;
#endif

    // 2. RISOLUZIONE DNS hostname -> IP
    struct hostent* host = gethostbyname(server_host);
    if (host == NULL) {
        printf("Errore: impossibile risolvere l'host %s.\n", server_host);
        clearwinsock();
        return -1;
    }

    struct in_addr server_ip = *(struct in_addr*)host->h_addr;

    // Creazione socket UDP
    SOCKET sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Struttura indirizzo server
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);   // Porta in network byte order
    serv_addr.sin_addr = server_ip;            // IP del server

    char send_buf[1 + MAX_CITY_LEN];           // [type][city]
    send_buf[0] = type;
    memset(send_buf + 1, 0, MAX_CITY_LEN);
    strncpy(send_buf + 1, city, MAX_CITY_LEN - 1);

    // Invio pacchetto
    sendto(sock, send_buf, sizeof(send_buf), 0,
           (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char recv_buf[9];                          // [status][type][value]
    struct sockaddr_in from_addr;
    int from_len = sizeof(from_addr);

    if (recvfrom(sock, recv_buf, sizeof(recv_buf), 0,
                 (struct sockaddr*)&from_addr, &from_len) > 0) {

        uint32_t net_status, net_value;
        float value;

        // Estrazione status
        memcpy(&net_status, recv_buf, 4);
        unsigned int status = ntohl(net_status);

        // Estrazione tipo
        char r_type = recv_buf[4];

        // Estrazione valore float
        memcpy(&net_value, recv_buf + 5, 4);
        net_value = ntohl(net_value);
        memcpy(&value, &net_value, 4);

        // risultato
        printf("Risposta dal server %s (ip %s). ",
               host->h_name, inet_ntoa(server_ip));

        if (status == STATUS_OK) {
            char* unit = (r_type == 't') ? "°C" :
                         (r_type == 'h') ? "%"  :
                         (r_type == 'w') ? " km/h" : " hPa";

            char* label = (r_type == 't') ? "Temperatura" :
                          (r_type == 'h') ? "Umidità"     :
                          (r_type == 'w') ? "Vento"       : "Pressione";

            printf("%s: %s = %.1f%s\n", city, label, value, unit);

        } else if (status == STATUS_CITY_NOT_FOUND) {
            printf("Città non disponibile\n");
        } else {
            printf("Richiesta non valida\n");
        }
    }

    // Chiusura socket
    closesocket(sock);
    clearwinsock();

    return 0;
}
