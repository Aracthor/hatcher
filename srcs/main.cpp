#include "hatcher/GameApplication.hpp"

int main(int argc, char** argv)
{
    hatcher::GameApplication demo(argv[0]);
    demo.StartRendering("hatcher - demo", 800, 600);
    return demo.Run();
}
