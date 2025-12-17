#include "protocol.h"
#include <time.h>
#include <stdint.h>       // Tipi a dimensione fissa

// Generazione temperatura casuale
float get_temperature(void) {
    return -10.0f + (rand() / (float)RAND_MAX) * 50.0f;
}

// Generazione umidità casuale
float get_humidity(void) {
    return 20.0f + (rand() / (float)RAND_MAX) * 80.0f;
}

// Generazione vento casuale
float get_wind(void) {
    return (rand() / (float)RAND_MAX) * 100.0f;
}

// Generazione pressione casuale
float get_pressure(void) {
    return 950.0f + (rand() / (float)RAND_MAX) * 100.0f;
}

// Elenco città
const char* supported_cities[] = {
    "Bari","Roma","Milano","Napoli","Torino",
    "Palermo","Genova","Bologna","Firenze","Venezia"
};

// Verifica la città
int is_supported_city(const char* city) {
    for (int i = 0; i < 10; i++) {
        if (strcasecmp(supported_cities[i], city) == 0)
            return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {

    srand(time(NULL));

    int port = SERVER_PORT;              // Porta default
    if (argc == 3 && strcmp(argv[1], "-p") == 0)
        port = atoi(argv[2]);

#if defined WIN32
    // Inizializzazione Winsock
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif

    // Creazione socket
    SOCKET sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Configurazione indirizzo server
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Associazione socket alla porta
    if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Bind fallito.\n");
        return -1;
    }

    printf("Server in ascolto sulla porta %d...\n", port);

    // Ciclo principale del server
    while (1) {

        struct sockaddr_in cli_addr;
        int cli_len = sizeof(cli_addr);
        char recv_buf[1 + MAX_CITY_LEN];

        // Ricezione richiesta dal client
        if (recvfrom(sock, recv_buf, sizeof(recv_buf), 0,
                     (struct sockaddr*)&cli_addr, &cli_len) > 0) {

            char type = recv_buf[0];     // Tipo richiesta
            char city[MAX_CITY_LEN];

            // nome città
            strncpy(city, recv_buf + 1, MAX_CITY_LEN - 1);
            city[MAX_CITY_LEN - 1] = '\0';

            // Reverse DNS
            struct hostent* host =
                gethostbyaddr((char*)&cli_addr.sin_addr,
                              sizeof(cli_addr.sin_addr), AF_INET);

            printf("Richiesta da %s (ip %s): type='%c', city='%s'\n",
                   (host ? host->h_name : "unknown"),
                   inet_ntoa(cli_addr.sin_addr),
                   type, city);

            unsigned int status;
            float val = 0.0f;

            // Validazione richiesta
            if (type != 't' && type != 'h' && type != 'w' && type != 'p')
                status = STATUS_INVALID_REQUEST;
            else if (!is_supported_city(city))
                status = STATUS_CITY_NOT_FOUND;
            else {
                status = STATUS_OK;
                if (type == 't') val = get_temperature();
                else if (type == 'h') val = get_humidity();
                else if (type == 'w') val = get_wind();
                else val = get_pressure();
            }

            // Serializzazione risposta
            char send_buf[9];
            uint32_t net_status = htonl(status);
            uint32_t net_value;

            memcpy(&net_value, &val, 4);
            net_value = htonl(net_value);

            memcpy(send_buf, &net_status, 4);
            send_buf[4] = type;
            memcpy(send_buf + 5, &net_value, 4);

            // Invio risposta al client
            sendto(sock, send_buf, 9, 0,
                   (struct sockaddr*)&cli_addr, cli_len);
        }
    }

    return 0;
}
