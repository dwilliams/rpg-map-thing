#ifndef MAP_THING_HEXMAP_H
#define MAP_THING_HEXMAP_H

#define _USE_MATH_DEFINES

//#include <cstdint>
//#include <vector>
//#include <string>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

#include "HexDetail.h"

// FIXME: Map ToDos:
//        - Convert to 2D array of HexTiles (new class)

class HexMap {
    public:
        explicit HexMap(int tiles_x, int tiles_y, int pixel_scale = 10);
        //void DrawMap(olc::PixelGameEngine& pge);
        void LoadMap(); // FIXME: This is a temp hack until a map loading system is implemented.
        void DrawMap(olc::TransformedView& tv);
        void FindTileByPixel(olc::vi2d pixel_coords);
        void SelectTileByPixel(olc::vi2d pixel_coords);

    private:
        olc::vi2d num_tiles;
        olc::vi2d pixel_start_offset;
        int scale;
        int factor_a;
        int factor_b;
        int factor_c;
        olc::vi2d hex_hover;
        std::vector<olc::vi2d> hexes_selected;
        std::vector<HexDetail> hex_details;

        //void DrawHex(olc::PixelGameEngine& pge, olc::vi2d hex_index);
        //void DrawSelected(olc::PixelGameEngine& pge, olc::vi2d hex_index);
        //void DrawHovered(olc::PixelGameEngine& pge, olc::vi2d hex_index);
        void DrawHex(olc::TransformedView& tv, olc::vi2d hex_index);
        void DrawSelected(olc::TransformedView& tv, olc::vi2d hex_index);
        void DrawHovered(olc::TransformedView& tv, olc::vi2d hex_index);
        bool CheckInsideHex(olc::vi2d hex_index, olc::vi2d pixel_coords);
        bool RightOfEdge(olc::vi2d edge_first_point, olc::vi2d edge_second_point, olc::vi2d test_point);
        void SelectHex(olc::vi2d hex_index);

        int GetIndexFromCoords(olc::vi2d coords);

        // FIXME: Convert this into some sort of map loading system.
        std::string worldHexes =
                "##############################"
                "#............##..........RR..#"
                "#............##..........RR..#"
                "#........................RR..#"
                "#............##..........RR..#"
                "####...#######################"
                "#............................#"
                "#............................#"
                "#............................#"
                "#............................#"
                "#######################...####"
                "#PPPPP.......................#"
                "#PPPP........................#"
                "#PP..........................#"
                "#P...........................#"
                "#############...##############"
                "#............................#"
                "#............................#"
                "#............................#"
                "##############################";
};

#endif //MAP_THING_HEXMAP_H
