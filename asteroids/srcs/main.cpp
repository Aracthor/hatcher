#include "hatcher/GameApplication.hpp"

using namespace hatcher;

int main(int argc, char** argv)
{
    GameApplication demo(argv[0]);
    demo.StartRendering("hatcher - asteroids", 800, 600);
    return demo.Run();
}
