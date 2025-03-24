#include "hatcher/GameApplication.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        hatcher::GameApplication demo(argc, argv);
        demo.StartRendering("hatcher - asteroids", 800, 600);
        return demo.Run();
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Fatal error: " << exception.what() << std::endl;
        return 2;
    }
}
