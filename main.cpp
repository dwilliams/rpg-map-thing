//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
//#include "spdlog/spdlog.h"
//#include "spdlog/sinks/stdout_color_sinks.h"

//#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPixelGameEngine.h"

#include "mainwindow.h"

int main()
{
    // Setup logging to console
    //auto main_logger = spdlog::stdout_color_mt("main_logger");
    //main_logger->set_level(spdlog::level::debug);
    //main_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [P:%P T:%t] [%^%l%$] %v");
    //main_logger->debug("This message should be displayed..");


    // Start the application
    MainWindow window;
    if (window.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1))
        window.Start();

    return 0;
}
