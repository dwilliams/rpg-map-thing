#ifndef MAP_THING_HEXDETAIL_H
#define MAP_THING_HEXDETAIL_H

#include "olcPixelGameEngine.h"

class HexDetail {
    public:
        bool walk_through = false; // can hex be walled through?
        bool see_through = false; // can hex be seen through?
        olc::Pixel base_color = olc::DARK_GREEN;
        olc::Pixel edge_color = olc::BLACK;
        // FIXME: Add a texture index.  This will require some sort of texture loader.
        // FIXME: How should things, such as items and traps, be handled?
};


#endif //MAP_THING_HEXDETAIL_H
