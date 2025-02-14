#pragma once

struct Color {
    float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
};

struct TextureParams {
    unsigned int target = 0;
    int level = 0;
    int internalFormat = 0;
    int border = 0;
    unsigned int format = 0;
    unsigned int dataType = 0;
};

struct RBConfig {
    unsigned int format = 0;
    unsigned int attachment = 0;
    unsigned int width = 0;
    unsigned int height = 0;
};

struct FBConfig {
    unsigned int operation = 0;
    unsigned int width = 0;
    unsigned int height = 0;
};
