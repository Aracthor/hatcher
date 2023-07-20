#pragma once

#include <GL/glew.h>

#ifdef NDEBUG
#include <iostream>
#define GL_CHECK(glFunction)                                                                                           \
    {                                                                                                                  \
        glFunction;                                                                                                    \
        int error = glGetError();                                                                                      \
        if (error != 0)                                                                                                \
            std::cerr << __FILE__ << ":" << __LINE__ << " " << #glFunction << " PANIK: "                               \
                      << "0x" << std::hex << error << std::dec << std::endl;                                           \
    }
#else
#define GL_CHECK(glFunction) glFunction;
#endif
