#ifndef __CLIENT_PROTOCOL_MESSAGES_
#define __CLIENT_PROTOCOL_MESSAGES_

// REGISTER: 0b01000000
#define REGISTER_CMD 0x40

// PLACE_BOMB: 0b1000000x
// x = 1 -- previous location
// x = 0 -- current location
#define PLACE_BOMB_CMD 0x80

// MOVE: 0b110000xx
// xx - move direction (00, 01, 10, 11)
#define MOVE_CMD 0xC0

#endif
