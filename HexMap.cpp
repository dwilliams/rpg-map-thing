#include "HexMap.h"

HexMap::HexMap(int tiles_x, int tiles_y, int pixel_scale) {
    num_tiles = olc::vi2d(tiles_x, tiles_y);
    scale = pixel_scale;
    // Using info from http://www.rdwarf.com/users/wwonko/hex/index.html to draw hexagons
    factor_a = scale / 2;
    factor_b = scale * 0.866025; // sin(60 degrees) is about 0.866025
    factor_c = factor_a + factor_a;
    pixel_start_offset = olc::vi2d(factor_c, factor_a);
    hex_hover = {-1, -1};
    printf("num_tiles_x: %d, num_tiles_y: %d, scale: %d, factor_a: %d, factor_b: %d, factor_c: %d\n",
           num_tiles.x, num_tiles.y, scale, factor_a, factor_b, factor_c);
    for(int i = 0; i < (num_tiles.x * num_tiles.y); i++) {
        hex_details.push_back(HexDetail());
    }
}

void HexMap::LoadMap() {
    // FIXME: This is a hack to convert the temp text string map format in to hexes.
    for(int i = 0; i < num_tiles.x; i++) {
        for(int j = 0; j < num_tiles.y; j++) {
            // FIXME: Load the tiles into a 2D array
            int index = GetIndexFromCoords(olc::vi2d(i, j));
            //HexDetail hex = HexDetail();
            HexDetail hex = hex_details.at(index);

            switch(worldHexes.at(index)) {
                case '.':
                    hex.walk_through = true;
                    hex.see_through = true;
                    hex.base_color = olc::Pixel(241, 204, 170);
                    hex.edge_color = olc::Pixel(230, 161, 99);
                    break;
                case 'P':
                    hex.walk_through = true;
                    hex.see_through = true;
                    hex.base_color = olc::VERY_DARK_BLUE;
                    hex.edge_color = olc::DARK_BLUE;
                    break;
                case 'R':
                    hex.walk_through = false;
                    hex.see_through = true;
                    hex.base_color = olc::GREY;
                    hex.edge_color = olc::DARK_GREY;
                    break;
                case '#':
                default:
                    hex.walk_through = false;
                    hex.see_through = false;
                    hex.base_color = olc::Pixel(74, 46, 21);
                    hex.edge_color = olc::Pixel(101, 67, 33); // DARK BROWN #654321
                    break;
            }
            hex_details[index] = hex;
        }
    }
}

//void HexMap::DrawMap(olc::PixelGameEngine& pge) {
//
//    for(int i = 0; i < num_tiles.x; i++) {
//        for(int j = 0; j < num_tiles.y; j++) {
//            DrawHex(pge, olc::vi2d(i, j));
//        }
//    }
//
//    // Draw selected hexes
//    for(olc::vi2d selected_hex : hexes_selected) {
//        DrawSelected(pge, selected_hex);
//    }
//
//    // Draw hovered hex
//    if(hex_hover.x > -1 && hex_hover.y > -1) {
//        DrawHovered(pge, hex_hover);
//    }
//}

void HexMap::DrawMap(olc::TransformedView& tv) {
    for(int i = 0; i < num_tiles.x; i++) {
        for(int j = 0; j < num_tiles.y; j++) {
            DrawHex(tv, olc::vi2d(i, j));
        }
    }

    // Draw selected hexes
    for(olc::vi2d selected_hex : hexes_selected) {
        DrawSelected(tv, selected_hex);
    }

    // Draw hovered hex
    if(hex_hover.x > -1 && hex_hover.y > -1) {
        DrawHovered(tv, hex_hover);
    }
}

void HexMap::FindTileByPixel(olc::vi2d pixel_coords) {
    // Find x & y, then check the hexes +/- 1
    int loc_x = (pixel_coords.x - pixel_start_offset.x) / (factor_b + factor_b);
    int loc_y = (pixel_coords.y - pixel_start_offset.y) / (factor_a + factor_c);
    //printf("Estimate X: %d, Y: %d\n", loc_x, loc_y);

    hex_hover = {-1, -1}; // default to none hovered

    for(int i = loc_x - 1; i < loc_x + 2; i++) {
        if(i < 0 || i >= num_tiles.x) {
            continue; // out of bounds, so skip ahead
        }
        for(int j = loc_y - 1; j < loc_y + 2; j++) {
            if(j < 0 || j >= num_tiles.y) {
                continue; // out of bounds, so skip ahead
            }
            // This could be unrolled and be a bit more performant, but maybe improve it later
            if(CheckInsideHex(olc::vi2d(i, j), pixel_coords)) {
                //printf("Hover Tile X: %d, Y: %d\n", i, j);
                hex_hover = {i, j};
            }
        }
    }
}

void HexMap::SelectTileByPixel(olc::vi2d pixel_coords) {
    // Find x & y, then check the hexes +/- 1
    int loc_x = (pixel_coords.x - pixel_start_offset.x) / (factor_b + factor_b);
    int loc_y = (pixel_coords.y - pixel_start_offset.y) / (factor_a + factor_c);
    //printf("Estimate X: %d, Y: %d\n", loc_x, loc_y);

    hex_hover = {-1, -1}; // default to none hovered

    for(int i = loc_x - 1; i < loc_x + 2; i++) {
        if(i < 0 || i >= num_tiles.x) {
            continue; // out of bounds, so skip ahead
        }
        for(int j = loc_y - 1; j < loc_y + 2; j++) {
            if(j < 0 || j >= num_tiles.y) {
                continue; // out of bounds, so skip ahead
            }
            // This could be unrolled and be a bit more performant, but maybe improve it later
            if(CheckInsideHex(olc::vi2d(i, j), pixel_coords)) {
                SelectHex(olc::vi2d(i, j));
            }
        }
    }
}

//void HexMap::DrawHex(olc::PixelGameEngine& pge, olc::vi2d hex_index) {
//    //printf("hex_index.x: %d, hex_index.y: %d\n", hex_index.x, hex_index.y);
//    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
//    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));
//    //printf("dim_x: %d ", dim_x);
//    //printf("dim_y: %d\n", dim_y);
//
//    olc::Pixel color = olc::BLACK;
//
//    // Calculate pixel coordinates of corners
//    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
//    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
//    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
//    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
//    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
//    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);
//
//    // Draw the Lines
//    pge.DrawLine(point_top, point_upper_right, color);
//    pge.DrawLine(point_upper_right, point_lower_right, color);
//    pge.DrawLine(point_lower_right, point_bottom, color);
//    pge.DrawLine(point_bottom, point_lower_left, color);
//    pge.DrawLine(point_lower_left, point_upper_left, color);
//    pge.DrawLine(point_upper_left, point_top, color);
//    // For testing, draw the map coordinates in the hex
//    //pge.DrawString(dim_x - factor_b + 5, dim_y + factor_a + 5, std::format("X: {}", hex_index.x), olc::DARK_BLUE);
//    //pge.DrawString(dim_x - factor_b + 5, dim_y + factor_c + 5, std::format("Y: {}", hex_index.y), olc::DARK_BLUE);
//}
//
//void HexMap::DrawSelected(olc::PixelGameEngine& pge, olc::vi2d hex_index) {
//    //printf("hex_index.x: %d, hex_index.y: %d\n", hex_index.x, hex_index.y);
//    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
//    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));
//    //printf("dim_x: %d ", dim_x);
//    //printf("dim_y: %d\n", dim_y);
//
//    olc::Pixel color_border = olc::BLUE;
//    olc::Pixel color_fill = olc::Pixel(0, 0, 255, 66);
//
//    // Calculate pixel coordinates of corners
//    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
//    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
//    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
//    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
//    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
//    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);
//
//    // Draw the Lines
//    pge.DrawLine(point_top, point_upper_right, color_border);
//    pge.DrawLine(point_upper_right, point_lower_right, color_border);
//    pge.DrawLine(point_lower_right, point_bottom, color_border);
//    pge.DrawLine(point_bottom, point_lower_left, color_border);
//    pge.DrawLine(point_lower_left, point_upper_left, color_border);
//    pge.DrawLine(point_upper_left, point_top, color_border);
//
//    // Alpha blend over the whole hex
//    pge.SetPixelMode(olc::Pixel::ALPHA);
//    pge.FillTriangle(point_upper_left, point_top, point_upper_right, color_fill);
//    olc::vi2d rect_size = olc::vi2d(point_upper_right.x - point_upper_left.x, point_lower_left.y - point_upper_left.y - 1);
//    pge.FillRect(point_upper_left.x, point_upper_left.y + 1, rect_size.x, rect_size.y, color_fill);
//    pge.FillTriangle(point_lower_left, point_lower_right, point_bottom, color_fill);
//    pge.SetPixelMode(olc::Pixel::NORMAL);
//}
//
//void HexMap::DrawHovered(olc::PixelGameEngine& pge, olc::vi2d hex_index) {
//    //printf("hex_index.x: %d, hex_index.y: %d\n", hex_index.x, hex_index.y);
//    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
//    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));
//    //printf("dim_x: %d ", dim_x);
//    //printf("dim_y: %d\n", dim_y);
//
//    olc::Pixel color = olc::YELLOW;
//
//    // Calculate pixel coordinates of corners
//    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
//    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
//    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
//    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
//    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
//    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);
//
//    // Draw the Lines
//    pge.DrawLine(point_top, point_upper_right, color);
//    pge.DrawLine(point_upper_right, point_lower_right, color);
//    pge.DrawLine(point_lower_right, point_bottom, color);
//    pge.DrawLine(point_bottom, point_lower_left, color);
//    pge.DrawLine(point_lower_left, point_upper_left, color);
//    pge.DrawLine(point_upper_left, point_top, color);
//
//}

void HexMap::DrawHex(olc::TransformedView& tv, olc::vi2d hex_index) {
    //printf("hex_index.x: %d, hex_index.y: %d\n", hex_index.x, hex_index.y);
    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));
    //printf("dim_x: %d ", dim_x);
    //printf("dim_y: %d\n", dim_y);

    HexDetail hex = hex_details.at(GetIndexFromCoords(hex_index));
    //olc::Pixel color = olc::BLACK;

    // Calculate pixel coordinates of corners
    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);

    // Fill, then outline... due to drawing always over top
    // FIXME:  There seems to be overlaps on the edges that can't be avoided.  Convert this to a rasterizer method?
    tv.FillTriangle(point_upper_left, point_top, point_upper_right, hex.base_color);
    olc::vi2d rect_size = olc::vi2d(point_upper_right.x - point_upper_left.x, point_lower_left.y - point_upper_left.y);
    tv.FillRect(point_upper_left.x, point_upper_left.y, rect_size.x, rect_size.y, hex.base_color);
    tv.FillTriangle(point_lower_left, point_lower_right, point_bottom, hex.base_color);

    // Draw the Lines
    tv.DrawLine(point_top, point_upper_right, hex.edge_color);
    tv.DrawLine(point_upper_right, point_lower_right, hex.edge_color);
    tv.DrawLine(point_lower_right, point_bottom, hex.edge_color);
    tv.DrawLine(point_bottom, point_lower_left, hex.edge_color);
    tv.DrawLine(point_lower_left, point_upper_left, hex.edge_color);
    tv.DrawLine(point_upper_left, point_top, hex.edge_color);
    // For testing, draw the map coordinates in the hex
    //pge.DrawString(dim_x - factor_b + 5, dim_y + factor_a + 5, std::format("X: {}", hex_index.x), olc::DARK_BLUE);
    //pge.DrawString(dim_x - factor_b + 5, dim_y + factor_c + 5, std::format("Y: {}", hex_index.y), olc::DARK_BLUE);
}

void HexMap::DrawSelected(olc::TransformedView& tv, olc::vi2d hex_index) {
    //printf("hex_index.x: %d, hex_index.y: %d\n", hex_index.x, hex_index.y);
    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));
    //printf("dim_x: %d ", dim_x);
    //printf("dim_y: %d\n", dim_y);

    olc::Pixel color_border = olc::BLUE;
    olc::Pixel color_fill = olc::Pixel(0, 0, 255, 66);

    // Calculate pixel coordinates of corners
    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);

    // Draw the Lines
    tv.DrawLine(point_top, point_upper_right, color_border);
    tv.DrawLine(point_upper_right, point_lower_right, color_border);
    tv.DrawLine(point_lower_right, point_bottom, color_border);
    tv.DrawLine(point_bottom, point_lower_left, color_border);
    tv.DrawLine(point_lower_left, point_upper_left, color_border);
    tv.DrawLine(point_upper_left, point_top, color_border);

    // Alpha blending over the whole hex
    tv.SetPixelMode(olc::Pixel::ALPHA);
    // FIXME:  There seems to be overlaps on the edges that can't be avoided.  Convert this to a rasterizer method?
    tv.FillTriangle(point_upper_left, point_top, point_upper_right, color_fill);
    //olc::vi2d rect_size = olc::vi2d(point_upper_right.x - point_upper_left.x, point_lower_left.y - point_upper_left.y - 1);
    olc::vi2d rect_size = olc::vi2d(point_upper_right.x - point_upper_left.x, point_lower_left.y - point_upper_left.y);
    //tv.FillRect(point_upper_left.x, point_upper_left.y + 1, rect_size.x, rect_size.y, color_fill);
    tv.FillRect(point_upper_left.x, point_upper_left.y, rect_size.x, rect_size.y, color_fill);
    tv.FillTriangle(point_lower_left, point_lower_right, point_bottom, color_fill);
    tv.SetPixelMode(olc::Pixel::NORMAL);
}

void HexMap::DrawHovered(olc::TransformedView& tv, olc::vi2d hex_index) {
    //printf("hex_index.x: %d, hex_index.y: %d\n", hex_index.x, hex_index.y);
    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));
    //printf("dim_x: %d ", dim_x);
    //printf("dim_y: %d\n", dim_y);

    olc::Pixel color = olc::YELLOW;

    // Calculate pixel coordinates of corners
    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);

    // Draw the Lines
    tv.DrawLine(point_top, point_upper_right, color);
    tv.DrawLine(point_upper_right, point_lower_right, color);
    tv.DrawLine(point_lower_right, point_bottom, color);
    tv.DrawLine(point_bottom, point_lower_left, color);
    tv.DrawLine(point_lower_left, point_upper_left, color);
    tv.DrawLine(point_upper_left, point_top, color);

}

bool HexMap::CheckInsideHex(olc::vi2d hex_index, olc::vi2d pixel_coords) {
    // calc hex points
    int dim_x = pixel_start_offset.x + (hex_index.x * (factor_b + factor_b)) + ((hex_index.y % 2) * factor_b); // add offset for odd number rows
    int dim_y = pixel_start_offset.y + (hex_index.y * (factor_a + factor_c));

    // Calculate pixel coordinates of corners
    olc::vi2d point_top = olc::vi2d(dim_x, dim_y);
    olc::vi2d point_upper_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a);
    olc::vi2d point_lower_left = olc::vi2d(dim_x - factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_upper_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a);
    olc::vi2d point_lower_right = olc::vi2d(dim_x + factor_b, dim_y + factor_a + factor_c);
    olc::vi2d point_bottom = olc::vi2d(dim_x, dim_y + factor_c + factor_c);

    // check edges
    // Using the technique from here:
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
    // Be careful of the x and y values in the equations.
    // All were false, so seems to need to wind the other way.
    bool r1 = RightOfEdge(point_upper_right, point_top, pixel_coords);
    bool r2 = RightOfEdge(point_lower_right, point_upper_right, pixel_coords);
    bool r3 = RightOfEdge(point_bottom, point_lower_right, pixel_coords);
    bool r4 = RightOfEdge(point_lower_left, point_bottom, pixel_coords);
    bool r5 = RightOfEdge(point_upper_left, point_lower_left, pixel_coords);
    bool r6 = RightOfEdge(point_top, point_upper_left, pixel_coords);

    // return result
    return r1 & r2 & r3 & r4 & r5 & r6;
}

bool HexMap::RightOfEdge(olc::vi2d edge_a_pt, olc::vi2d edge_b_pt, olc::vi2d test_pt) {
    return ((test_pt.x - edge_a_pt.x) * (edge_b_pt.y - edge_a_pt.y) - (test_pt.y - edge_a_pt.y) * (edge_b_pt.x - edge_a_pt.x) >= 0);
}

void HexMap::SelectHex(olc::vi2d hex_index) {
    bool to_select = true;
    for(int i = hexes_selected.size() - 1; i >= 0; i--) {
        if(hexes_selected.at(i) == hex_index) {
            hexes_selected.erase(hexes_selected.begin() + i); // FIXME: There has to be a better way to do this...
            to_select = false;
            //printf("Unselected hex #%d: %d, %d\n", i, hex_index.x, hex_index.y);
        }
    }
    if(to_select) {
        // None were removed, so select hex
        hexes_selected.push_back(hex_index);
        //printf("Selected hex: %d, %d\n", hex_index.x, hex_index.y);
    }
}

int HexMap::GetIndexFromCoords(olc::vi2d coords) {
    return (num_tiles.x * coords.y) + coords.x;
}