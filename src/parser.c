#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/communication.h"

#define LISTEN_PORT 8080

int main(){
    init_comm_network();
    SOCKET sock = create_udp_socket();

    //Bind the socket to the port
    bind_udp_socket(sock, LISTEN_PORT);

    Telemetry_Packet packet;
    int next_seq = -1;

    printf("Earth base receiving data on port %d\n ", LISTEN_PORT);

    while(1){
        //cleanup packet memory
        memset(&packet, 0, sizeof(Telemetry_Packet));

        //received bytes
        int bytes = get_telemetry(sock, &packet);

        //if data is received
        if(bytes > 0){
            //Convert from BIG ENDIAN to LITTLE ENDIAN
            uint16_t raw_pid = ntohs(packet.header.packet_ID);
            uint16_t raw_seq_control = ntohs(packet.header.packet_SC);
            uint16_t raw_packet_DL = ntohs(packet.header.packet_DL);

            //Data extraction
            uint16_t apid = raw_pid & 0x07FF;
            
            //isolate the 2 bit sequence of sequence flags
            uint16_t seq_flag = (raw_seq_control >> 14) & 0x03;
            //extract sequence counter
            uint16_t seq_counter = raw_seq_control & 0x3FFF;
            //extract data length
            uint16_t packet_DL = raw_packet_DL + 1;

            //Sequence check
            if (next_seq != -1 && seq_counter != next_seq) {
                printf("Lost sequence, expected to receive: %d, received: %d\n", next_seq, seq_counter);
            }

            printf("APID: %d | SEQ: %d | Len: %d bytes | Payload: %s\n", apid, seq_counter, packet_DL, packet.data_payload);

            next_seq = (seq_counter + 1) & 0x3FFF;

        }
    }

    cleanup_comm_network();
    return 0;
}