#include <Zbuffer.hpp>

Zbuffer::Zbuffer(int width, int height)
    : width(width), height(height), bufferArray(new float[width * height])
// initialize bufferArray with width*height pixels
{
}

Zbuffer::~Zbuffer()
{
    delete[] bufferArray;
    bufferArray = nullptr;
}

void Zbuffer::Clear()
{
    const int allDepths = width * height;
    for (int i = 0; i < allDepths; i++)
    {
        bufferArray[i] = -1 * std::numeric_limits<float>::infinity();
        // bufferArray[i] = 0;
        // At first every pixel has infinite depth i.e. the first pixel drawn is always at front
    }
}

float &Zbuffer::At(int x, int y)
{
    // assert(x >= 0);     //
    // assert(x < width);  // should be inside the viewport
    // assert(y >= 0);     //
    // assert(y < height); //
    // if( x>=0 && x<width && y>=0 && y<height )
    return bufferArray[y * width + x];
    // float retVal = 100.f;
    // return retVal;
    // search for the pixel depth
}

bool Zbuffer::testAndSet(int x, int y, float depth)
{
    if (depth >= bufferArray[y * width + x]) // we just take care of the element with lowest
    {                                        // depth we dont care about other elements in that
        bufferArray[y * width + x] = depth;  // pixel, we get lowest depth and we rasterize it, that's all
        return true;
    }
    return false;
}