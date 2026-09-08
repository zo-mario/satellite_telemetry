#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/communication.h"

//manage multiplatform waiting time
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(x) Sleep(x)
#else
    #include <unistd.h>
    #define SLEEP_MS(x) usleep((x) * 1000)
#endif

#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 8080
#define APID_THERMAL 10 //ID to pretend we have a thermal sensor on board

int main(){
    init_comm_network();
    SOCKET sock = create_udp_socket();

    Telemetry_Packet packet;
    uint16_t seq_counter = 0;
    
    printf("Satellite communication started with %s:%d\n ", TARGET_IP, TARGET_PORT);

    while(1){
        //cleanup packet memory
        memset(&packet, 0, sizeof(Telemetry_Packet));

        //HEADER COMPOSITION
        uint16_t packet_version = 0; //3 bit
        uint16_t packet_type = 0; // 1 bit
        uint16_t second_handler = 0; // 1 bit

        uint16_t raw_pid = (packet_version << 13) | (packet_type << 12) | (second_handler << 11) | (APID_THERMAL & 0x07FF);

        // Sequence flags = 3
        uint16_t raw_seq_control = (3 << 14) | (seq_counter & 0x3FFF);

        uint16_t raw_packet_DL = PAYLOAD_LENGTH - 1;

        //BIG-ENDIAN CONVERSION
        packet.header.packet_ID = htons(raw_pid);
        packet.header.packet_SC = htons(raw_seq_control);
        packet.header.packet_DL = htons(raw_packet_DL);

        //PAYLOAD
        sprintf((char *)packet.data_payload, "T_board: 2%d.5 C", rand() % 10);

        //SENDING TELEMETRY
        if(send_telemetry(sock, TARGET_IP, TARGET_PORT, &packet) > 0){
            printf("[TM] Inviato pacchetto | APID: %d | SEQ: %d\n", APID_THERMAL, seq_counter);
        }

        //increment the seq counter, limited at 14 bit as required by ccsds standard
        seq_counter = (seq_counter + 1) & 0x3FFF;

        //Wait 1 second before sending the next packet
        SLEEP_MS(1000);
    }

    cleanup_comm_network();
    return 0;
}