# Template Esonero TCP - Reti di Calcolatori

Template repository per l'assegnazione di programmazione client-server TCP.
Corso di Reti di Calcolatori - ITPS A-L

## Descrizione

Questo repository contiene la struttura base per lo sviluppo di un'applicazione client-server TCP in linguaggio C, utilizzando la libreria standard delle socket. Il codice è progettato per essere portabile su sistemi operativi **Windows**, **Linux** e **macOS**.

## Struttura del Repository

Il repository è organizzato in due progetti Eclipse CDT separati:

```
.
├── client-project/          # Progetto Eclipse per il client
│   ├── .project            # Configurazione progetto Eclipse
│   ├── .cproject           # Configurazione Eclipse CDT
│   └── src/
│       ├── main.c          # File principale del client
│       └── header.h        # Header con definizioni e prototipi
│
└── server-project/          # Progetto Eclipse per il server
    ├── .project            # Configurazione progetto Eclipse
    ├── .cproject           # Configurazione Eclipse CDT
    └── src/
        ├── main.c          # File principale del server
        └── header.h        # Header con definizioni e prototipi
```

## Come Utilizzare il Template

### 1. Creare la propria copia del repository

1. Cliccare su "Use this template" in alto a destra su GitHub
2. Creare un nuovo repository personale
3. Clonare il repository sul proprio computer:
   ```bash
   git clone <url-del-tuo-repository>
   ```

### 2. Importare i progetti in Eclipse

1. Aprire **Eclipse CDT**
2. Selezionare `File → Import → General → Existing Projects into Workspace`
3. Selezionare la directory `client-project`
4. Ripetere i passi 2-3 per `server-project`

### 3. Compilazione su diversi sistemi operativi

#### Linux / macOS
Il progetto è configurato per compilare direttamente con GCC.

#### Windows
Assicurarsi di avere installato:
- MinGW o MinGW-w64
- Eclipse CDT configurato per utilizzare il compilatore MinGW

La libreria Winsock2 sarà automaticamente inizializzata grazie al codice boilerplate.

## File Principali

### header.h
Contiene:
- **Costanti condivise**: numero di porta del server, dimensione buffer, ecc.
- **Prototipi delle funzioni**: inserire qui le firme di tutte le funzioni implementate

Esempio:
```c
#define SERVER_PORT 27015
#define BUFFER_SIZE 512

// Prototipo funzione esempio
int connect_to_server(const char* server_address);
```

### main.c (Client e Server)
Contiene:
- **Codice boilerplate** per la portabilità cross-platform
- **Inizializzazione Winsock** su Windows
- **Sezioni TODO** dove implementare la logica dell'applicazione

## Portabilità Windows/Linux/macOS

Il codice utilizza direttive di preprocessore per gestire le differenze tra sistemi operativi:

```c
#if defined WIN32
    // Codice specifico per Windows (Winsock)
#else
    // Codice per Linux/macOS (BSD sockets)
#endif
```

### Principali differenze gestite:
- **Header files**: `winsock.h` su Windows vs `sys/socket.h`, `arpa/inet.h` su Unix
- **Inizializzazione**: WSAStartup/WSACleanup richiesti solo su Windows
- **Chiusura socket**: `closesocket()` su Windows, `close()` su Unix (definito tramite macro)

## Implementazione dell'Assegnazione

Gli studenti devono:

1. **Definire i parametri** nel file `header.h`:
   - Numero di porta dell'applicazione
   - Dimensione dei buffer
   - Altre costanti necessarie

2. **Implementare le funzioni** necessarie:
   - Aggiungere i prototipi nel file `header.h`
   - Implementare le funzioni nel file `main.c`

3. **Completare il codice** seguendo le sezioni TODO:
   - **Client**: creazione socket, connessione al server, invio/ricezione dati
   - **Server**: creazione socket, bind, listen, accept, gestione client

## Note Importanti

- Non modificare il codice boilerplate per la portabilità
- Testare il codice su almeno due sistemi operativi diversi se possibile
- Utilizzare le costanti definite in `header.h` invece di valori hardcoded
- Gestire correttamente gli errori delle chiamate di sistema
- Chiudere sempre i socket aperti prima di terminare

## Compilazione da Terminale (Opzionale)

### Linux / macOS
```bash
cd client-project/src
gcc -o client main.c

cd server-project/src
gcc -o server main.c
```

### Windows (MinGW)
```bash
cd client-project/src
gcc -o client.exe main.c -lws2_32

cd server-project/src
gcc -o server.exe main.c -lws2_32
```

## Risorse Utili

- [Guida alle Socket Berkeley](https://beej.us/guide/bgnet/)
- [Documentazione Winsock](https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2)
- [Man pages Linux per socket](https://man7.org/linux/man-pages/man2/socket.2.html)

## Licenza

Vedere il file [LICENSE](LICENSE) per i dettagli.
