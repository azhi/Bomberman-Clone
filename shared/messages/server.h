#ifndef __SERVER_PROTOCOL_MESSAGES_
#define __SERVER_PROTOCOL_MESSAGES_

// FULL_STATE_DUMP: 0x10
// FORMAT:
// field:
// [0bxx], xx - field type (01, 10, 11), count - MAP_WIDTH * MAP_HEIGHT,
//         order - right & down, align - aligned to byte
// characters:
// 0xCC [0xID 0xXX 0xYY 0b0000KLLL], CC - characters count, ID - character ID,
//                                   XX - character X coord, YY - character Y coord,
//                                   K - killed flag, LLL - last move
// bombs:
// 0xCC [0xID 0xXX 0xYY], CC - bombs count, ID - bomb ID,
//                        XX - bomb X coord, YY - bomb Y coord
#define FULL_STATE_DUMP_CMD 0x10

// CREATE_CHARACTER: 0x20
// FORMAT:
// 0xID 0xXX 0xYY 0b00000LLL, ID - character ID, XX - character X coord,
//                            YY - character Y coord, LLL - last move
#define CREATE_CHARACTER_CMD 0x20

// DELETE_CHARACTER: 0x30
// FORMAT:
// 0xID, ID - character ID
#define DELETE_CHARACTER_CMD 0x30

// MOVE_CHARACTER: 0x40
// FORMAT:
// 0xID 0xXX 0xYY 0b00000LLL, ID - character ID, XX - character X coord,
//                            YY - character Y coord, LLL - last move
#define MOVE_CHARACTER_CMD 0x40

// KILL_CHARACTER: 0x50
// FORMAT:
// [0xID], ID - character ID
#define KILL_CHARACTER_CMD 0x50

// PLACE_BOMB: 0x60
// FORMAT:
// 0xID 0xXX 0xYY, ID - bomb ID, XX - bomb X coord, YY - bomb Y coord
#define PLACE_BOMB_CMD 0x60

// DESTRUCT CELL: 0x70
// FORMAT:
// [0xXX 0xYY], XX - cell X coord, YY - cell Y coord
#define DESTRUCT_CELL_CMD 0x70

#endif
