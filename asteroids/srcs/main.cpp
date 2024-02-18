#include "hatcher/GameApplication.hpp"

using namespace hatcher;

int main(int argc, char** argv)
{
    GameApplication demo(argc, argv);
    demo.StartRendering("hatcher - asteroids", 800, 600);
    return demo.Run();
}
