#ifndef CCSDS_H
#define CCSDS_H

#include <stdint.h>
#include <stdlib.h>

#define PAYLOAD_LENGTH 128  //length in octets of the payload

//set the byte alignment and set pack to 1 (there will be no padding between the struct elements)
#pragma pack(push, 1)

//define the primary header struct: 6 octets length
typedef struct{
    //packet identification: 2 octets length
    //3 bit packet version | 1 bit packet type | 1 bit second handler flag | 11 bit application process identifier
    uint16_t packet_ID;

    //packet sequence control: 2 octets length
    //2 bits sequence flag | 14 bits packet name
    uint16_t packet_SC;

    //packet data length: 2 octets length 
    uint16_t packet_DL;

} CCSDS_PACKET_HEADER;

//packet structure
typedef struct{
    CCSDS_PACKET_HEADER header;

    //actual data in the packet
    uint8_t data_payload[PAYLOAD_LENGTH];

} Telemetry_Packet;

//set the memory allignment back to the standard
#pragma pack(pop)


#endif