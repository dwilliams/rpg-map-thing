#include "mainwindow.h"
#include "olcPixelGameEngine.h"

MainWindow::MainWindow() {
    sAppName = "Map Thing";
    map = std::make_shared<HexMap>(30, 20, 25);
}

bool MainWindow::OnUserCreate() {
    //tv = olc::TransformedView(olc::vi2d(MAP_VIEW_WIDTH, MAP_VIEW_HEIGHT), olc::vi2d(1, 1));
    tv.SetViewArea(olc::vi2d(MAP_VIEW_WIDTH, MAP_VIEW_HEIGHT));
    map->LoadMap();
    return true;
}

bool MainWindow::OnUserUpdate(float fElapsedTime) {
    // Get User Input
    //olc::vi2d mouse_coords = olc::vi2d(GetMouseX(), GetMouseY()); // This will need to be fixed for the transformed view
    olc::vi2d mouse_coords = tv.ScreenToWorld(GetMousePos());
    map->FindTileByPixel(mouse_coords);

    if(GetMouse(0).bPressed) {
        map->SelectTileByPixel(mouse_coords);
    }

    // Handling panning and zooming TransformedView
    if(GetMouse(1).bPressed) {
        tv.StartPan(GetMousePos());
    }
    if(GetMouse(1).bHeld) {
        tv.UpdatePan(GetMousePos());
    }
    if(GetMouse(1).bReleased) {
        tv.EndPan(GetMousePos());
    }
    if(GetMouseWheel() > 0) {
        tv.ZoomAtScreenPos(2.0f, GetMousePos());
    }
    if(GetMouseWheel() < 0) {
        tv.ZoomAtScreenPos(0.5f, GetMousePos());
    }

    // Calc Things

    // Start Drawing
    Clear(olc::DARK_GREEN);

    //map->DrawMap(*this);
    map->DrawMap(tv);

    // FIXME: Should move this to a user interface class
    olc::Pixel ui_edge_color = olc::WHITE;
    olc::Pixel ui_fill_color = olc::Pixel(255, 255, 255, 150);
    SetPixelMode(olc::Pixel::ALPHA);
    FillRect(10, 10, 200, 400, ui_fill_color);
    SetPixelMode(olc::Pixel::NORMAL);
    DrawRect(10, 10, 200, 400, ui_edge_color);
    SetPixelMode(olc::Pixel::ALPHA);
    FillRect(10, 420, 200, 200, ui_fill_color);
    SetPixelMode(olc::Pixel::NORMAL);
    DrawRect(10, 420, 200, 200, ui_edge_color);

    return true;
}