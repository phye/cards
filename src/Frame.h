#ifndef _FRAME_H_
#define _FRAME_H_
#include <stdint.h>

/*****
    Frame definition

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                     Header Magic Number                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Source |  Rev  |  Frame  Type  |  Data Length  | Padding       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   | Padding       |  Padding      |  Frame Tag                    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                        ... Data ...                           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    CRC        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    

    Header Magic Number (32b): 0xCAFEDADD
    Source Node			( 4b): Node number of the source
    Rev				    ( 4b): Software Revision, abcd.efgh
    Frame Type			( 8b): Frame Type indicates the purpose of this frame, 64 types max
    Data Length			( 8b): Length of the "Data" field, the unit is Byte
    Padding                     (24b): For extension, set to 0 if not used
    Frame Tag			(16b): Tag of this frame
    Data		   (256b max): Payload of the frame, defined by each Frame Type
    CRC					( 8b): 8bit CRC checksum of the frame
***/

typedef struct _FrameHead_t {
    uint32_t frm_magic_num; 
    uint8_t frm_src     :4;
    uint8_t frm_ver     :4;
    uint8_t frm_type;
    uint8_t frm_len;
    uint8_t frm_pad1;     //One byte padding
    uint16_t frm_pad2;    //Two bytes padding
    uint16_t frm_tag;
} FrameHead_t;

typedef enum _FrameType_t {
    DISPATCH_CARD = 0,
    CLAIM_PRIME,        //Sent from client
    BANKER_NOTIF,
    SWAP_CARD_NOTIF,    //Sent from server
    SWAP_CARD_DATA,     //Sent from client
    SND_CARD_NOTIF,
    SND_CARD_DATA,      //Sent from client
    ROUND_RESULT_NOTIF,
    SET_RESULT_NOTIF,
    BCAST_START,
    CLAIM_PRIME_BCAST,
    SND_CARD_DATA_BCAST,
    SND_CARD_INVAL_DATA_BCAST, //Used for Invalid card sent 
    ACK_START,
    DISPATCH_CARD_ACK, 
    CLAIM_PRIME_ACK,
    CLAIM_PRIME_NACK,   //Invalid prime request
    CLAIM_PRIME_BCAST_ACK,
    BANKER_NOTIF_ACK,
    SWAP_CARD_NOTIF_ACK,
    SWAP_CARD_DATA_ACK,
    SND_CARD_NOTIF_ACK,
    SND_CARD_DATA_ACK,
    SND_CARD_DATA_NACK,
    SND_CARD_DATA_BCAST_ACK,
    SND_CARD_INVAL_DATA_BCAST_ACK,
    ROUND_RESULT_NOTIF_ACK,
    SET_RESULT_NOTIF_ACK,
    FRAME_TYPE_MAX
}FrameType_t;

#endif
