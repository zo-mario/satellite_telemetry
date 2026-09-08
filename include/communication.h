#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#ifdef _WIN32
//windows compiler branch
    #include <winsock.h>
    #include <WS2tcpip.h>

    #pragma comment(lib, "ws2_32.lib")

#else
//linux and macos compiler branch
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    //uniform standard linux types to windows names
    typedef int SOCKET;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

#include "ccsds.h" //need to know the Telemetry_Packet type

//communication network setup for Windows prototypes
void init_comm_network();
void cleanup_comm_network();

//UDP socket prototypes
SOCKET create_udp_socket();
void bind_udp_socket(SOCKET sock, int port);

//Earth-satellite communication methods prototypes
int send_telemetry(SOCKET sock, const char *target_ip, int target_port, Telemetry_Packet *packet);
int get_telemetry(SOCKET sock, Telemetry_Packet *packet);

#endif