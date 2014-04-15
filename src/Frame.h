#ifndef _FRAME_H_
#define _FRAME_H_
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
    int frm_magic_num; 
    int frm_src     :4;
    int frm_ver     :4;
    unsigned char frm_type;
    unsigned char frm_len;
    unsigned char frm_pad1;     //One byte padding
    unsigned short frm_pad2;    //Two bytes padding
    unsigned short frm_tag;
} FrameHead_t;

typedef enum _FrameType_t {
    DISPATCH_CARD = 0,
    DISPATCH_CARD_ACK, 
    CLAIM_PRIME,        //Sent from client
    CLAIM_PRIME_ACK,
    CLAIM_PRIME_NACK,   //Invalid prime request
    CLAIM_PRIME_BCAST,
    CLAIM_PRIME_BCAST_ACK,
    BANKER_NOTIF,
    BANKER_NOTIF_ACK,
    SWAP_CARD_NOTIF,    //Sent from server
    SWAP_CARD_NOTIF_ACK,
    SWAP_CARD_DATA,     //Sent from client
    SWAP_CARD_DATA_ACK,
    SND_NOTIF,
    SND_NOTIF_ACK,
    SND_CARD,      //Sent from client
    SND_CARD_ACK,
    SND_CARD_NACK,
    SND_CARD_BCAST,
    SND_CARD_BCAST_ACK,
    SNDROUND_RESULT_NOTIF,
    ROUND_RESULT_NOTIF_ACK,
    GAME_RESULT_NOTIF,
    GAME_RESULT_NOTIF_ACK
}FrameType_t;

#endif
