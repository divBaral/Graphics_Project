#pragma once

#include <limits>
#include <cassert>
#include <iostream>

class Zbuffer
{
public:
    Zbuffer(int, int);

    ~Zbuffer();

    void Clear();

    float &At(int, int);

    bool testAndSet(int, int, float);

private:
    int width;
    int height;
    float *bufferArray = nullptr;
};