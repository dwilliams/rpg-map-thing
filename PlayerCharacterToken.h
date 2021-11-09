#ifndef MAP_THING_PLAYERCHARACTERTOKEN_H
#define MAP_THING_PLAYERCHARACTERTOKEN_H

#include "Token.h"

class PlayerCharacterToken : public Token {
    public:
        int pc_id; // Identifier to link back to player character info

};

#endif //MAP_THING_PLAYERCHARACTERTOKEN_H
