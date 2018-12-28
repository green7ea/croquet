#include "error.h"

#include <GL/gl.h>
#include <cstdio>

void checkError(const char *location)
{
    const int error = glGetError();

    if (error != GL_NO_ERROR)
    {
        fprintf(stderr, "%s\n", location);

        switch (error)
        {
        case GL_INVALID_ENUM:
            fprintf(stderr, "\tGL_INVALID_ENUM\n");
            break;
        case GL_INVALID_VALUE:
            fprintf(stderr, "\tGL_INVALID_VALUE\n");
            break;
        case GL_INVALID_OPERATION:
            fprintf(stderr, "\tGL_INVALID_OPERATION\n");
            break;
        case GL_OUT_OF_MEMORY:
            fprintf(stderr, "\tGL_OUT_OF_MEMORY\n");
            break;
        default:
            fprintf(stderr, "\tUnknown error type\n");
            break;
        }
    }
}
