#ifndef MAP_THING_MAINWINDOW_H
#define MAP_THING_MAINWINDOW_H

//#include <cstdint>
//#include <vector>
//#include <string>
//#include <stdio.h>
//#include <stdlib.h>

//#include "spdlog/spdlog.h"

#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

#include "HexMap.h"

// These are defaults, but the program should allow resizing (user or longer disk list)
//#define WINDOW_WIDTH 640
//#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define MAP_VIEW_WIDTH 700
#define MAP_VIEW_HEIGHT 400

class MainWindow : public olc::PixelGameEngine
{
public:
    MainWindow();

    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

protected:

private:
    std::shared_ptr<HexMap> map;
    olc::TransformedView tv;
};


#endif //MAP_THING_MAINWINDOW_H
