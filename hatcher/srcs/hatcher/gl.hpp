#pragma once

#include <GL/glew.h>

#ifdef NDEBUG
#include <iostream>
#define GL_CHECK(glFunction)                                                                       \
    {                                                                                              \
        glFunction;                                                                                \
        int error = glGetError();                                                                  \
        if (error != 0)                                                                            \
            std::cerr << #glFunction << " PANIK: " << error << std::endl;                          \
    }
#else
#define GL_CHECK(glFunction) glFunction;
#endif
