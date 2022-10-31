//========================================================================
// OpenGL ES 2.0 triangle example
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#define GLAD_GLES2_IMPLEMENTATION
#include <glad/gles2.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "getopt.h"

typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;

static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

static const char* vertex_shader_text =
"#version 100\n"
"precision mediump float;\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 100\n"
"precision mediump float;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void usage(void)
{
    printf("Usage: triangle-test [OPTION]...\n");
    printf("Options:\n");
    printf("  -w, --width=WIDTH        the window width in pixels, default: 1024\n");
    printf("  -g, --height=HEIGHT      the window height in pixels, default: 768\n");
    printf("  -x, --cell-in-x=X        the number of grid cells in horizontal, default: 16\n");
    printf("  -y, --cell-in-y=Y        the number of grid cells in vertical, default: 12\n");
    printf("  -z, --layers=Z           the number of layers, default: 4\n");
    printf("  -h, --help               show this help\n");
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void parse_int(int* n, char* name)
{
    int t = atoi(optarg);
    if (t > 0)
    {
        *n = t;
    }
    else
    {
        fprintf(stderr, "`%s` must be a number greater than zero!\n", name);
    }
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    int width = 1024; // window width in pixels
    int height = 768; // window height in pixels
    int x = 16;       // number of grid cells in horizontal
    int y = 12;       // number of grid cells in vertical
    int z = 4;        // number of layers

    int ch;

    enum {
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        CELLS_IN_X,
        CELLS_IN_Y,
        LAYERS,
        HELP
    };

    const struct option options[] =
    {
        { "width",              1, NULL, WINDOW_WIDTH },
        { "height",             1, NULL, WINDOW_HEIGHT },
        { "cells-in-x",         1, NULL, CELLS_IN_X },
        { "cells-in-y",         1, NULL, CELLS_IN_Y },
        { "layers",             1, NULL, LAYERS },
        { "help",               0, NULL, HELP },
        { NULL, 0, NULL, 0 }
    };

    while ((ch = getopt_long(argc, argv, "w:g:x:y:z:h", options, NULL)) != -1)
    {
        switch (ch)
        {
        case 'w':
        case WINDOW_WIDTH:
            if (strcmp(optarg, "-") != 0)
            {
                parse_int(&width, "width");
            }
            break;
        case 'g':
        case WINDOW_HEIGHT:
            if (strcmp(optarg, "-") != 0)
            {
                parse_int(&height, "height");
            }
            break;
        case 'x':
        case CELLS_IN_X:
            if (strcmp(optarg, "-") != 0)
            {
                parse_int(&x, "cells-in-x");
            }
            break;
        case 'y':
        case CELLS_IN_Y:
            if (strcmp(optarg, "-") != 0)
            {
                parse_int(&y, "cells-in-y");
            }
            break;
        case 'z':
        case LAYERS:
            if (strcmp(optarg, "-") != 0)
            {
                parse_int(&z, "layers");
            }
            break;
        case 'h':
        case HELP:
            usage();
            exit(EXIT_SUCCESS);
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
        }
    }

    printf("window width: %d\n", width);
    printf("window height: %d\n", height);
    printf("cells in x: %d\n", x);
    printf("cells in y: %d\n", y);
    printf("layers: %d\n", z);

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL ES 2.0 Triangle (EGL)", NULL, NULL);
    if (!window)
    {
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
        window = glfwCreateWindow(640, 480, "OpenGL ES 2.0 Triangle", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLES2(glfwGetProcAddress);
    glfwSwapInterval(1);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

