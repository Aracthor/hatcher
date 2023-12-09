#pragma once

struct MeshComponent
{
    enum Identifier : unsigned int
    {
        Asteroid,
        Player,
        Shoot,
        UFO,
        WreckageAsteroid,
        WreckageShip,
        COUNT,
    };
    unsigned int ID;
    float scale;
};
