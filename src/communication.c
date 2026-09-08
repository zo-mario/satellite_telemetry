#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/communication.h"

//initialize Winsock on Windows
void init_comm_network() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Errore CRITICO: Impossibile inizializzare Winsock.\n");
        exit(EXIT_FAILURE);
    }
#endif
}

//cleans up network resources on windows
void cleanup_comm_network(){
    #ifdef _WIN32
    WSACleanup();
    #endif
}


//create udp socket
SOCKET create_udp_socket() {
    //create a new network istance | Address family IPv4 | Socket type | Protocol (UDP)
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Errore: Creazione socket UDP fallita.\n");
        exit(EXIT_FAILURE);
    }
    return sock;
}


//Assign the socket to a specific port
void bind_udp_socket(SOCKET sock, int port){
    //creates a specific container to configure IPv4 adresses 
    struct sockaddr_in server_address;
    //polish the local variable
    memset(&server_address, 0, sizeof(server_address));

    //specifies IPv4 addresses
    server_address.sin_family = AF_INET;
    //listens to all network interfaces
    server_address.sin_addr.s_addr = INADDR_ANY;
    //converts the port to big-endian
    server_address.sin_port = htons(port);

    if(bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR){
        fprintf(stderr, "Error: Impossible to bind the port %d\n", port);
        exit(EXIT_FAILURE);
    }
}

//send a ccsds packet to an address and a port
int send_telemetry(SOCKET sock, const char *target_ip, int target_port, Telemetry_Packet *packet){
    //creates a specific container to configure IPv4 adresses 
    struct sockaddr_in dest_address;
    //polish the local variable
    memset(&dest_address, 0, sizeof(dest_address));

    //specifies IPv4 addresses
    dest_address.sin_family = AF_INET;
    //converts the port to big-endian
    dest_address.sin_port = htons(target_port);

    //converts IP address from string to bytes | IPv4 | target address | result destination
    if (inet_pton(AF_INET, target_ip, &dest_address.sin_addr) <= 0) {
        fprintf(stderr, "Error: destination IP address not valid.\n");
        return -1;
    }

    //number of bytes sent
    int bytes = sendto(sock, (const char *)packet, sizeof(Telemetry_Packet), 0, (struct sockaddr*)&dest_address, sizeof(dest_address));

    return bytes;
}

//receive the bytes and puts them inside the data structure
int get_telemetry(SOCKET sock, Telemetry_Packet *packet){
    struct sockaddr_in sender_addr;

    // Gestione compatibilità tipo per la lunghezza dell'indirizzo tra Linux e Windows
    #ifdef _WIN32
        int sender_len = sizeof(sender_addr);
    #else
        socklen_t sender_len = sizeof(sender_addr);
    #endif

    //number of bytes received
    int bytes = recvfrom(sock, (char *)packet, sizeof(Telemetry_Packet), 0, (struct sockaddr*)&sender_addr, &sender_len);
}