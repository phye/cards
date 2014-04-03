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





#endif