#pragma once

#include <limits>
#include <cassert>
#include <iostream>

class Zbuffer
{
public:
    Zbuffer(int width, int height);

    ~Zbuffer()
    {
        delete[] bufferArray;
        bufferArray = nullptr;
    }

    void Clear();

    float &At(int x, int y);

    bool TestAndSet(int x, int y, float depth);

private:
    int width;
    int height;
    float *bufferArray = nullptr;
};