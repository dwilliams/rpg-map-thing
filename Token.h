#ifndef MAP_THING_TOKEN_H
#define MAP_THING_TOKEN_H

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class Token {
    public:
        olc::vi2d hex_location;

        virtual void Draw(olc::TransformedView& tv); // FIXME: Should this be handled by the token, the map, or the window?
};

#endif //MAP_THING_TOKEN_H
