#include "hatcher/GameApplication.hpp"

int main(int argc, char** argv)
{
    hatcher::GameApplication demo(argc, argv);
    demo.StartRendering("hatcher - demo", 800, 600);
    return demo.Run();
}
