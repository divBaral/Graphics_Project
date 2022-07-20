#pragma once
#include <tuple>
#include <vector>
#include <algorithm>


//rasterize triangle
template <typename T>
void RasterizeTriangle( T& p0, T& p1, T& p2, auto&& GetXY, auto&& Makeslope, auto&& DrawScanLine, auto&& Plot)
{   
    //unpacking the coordinates of the triangle
    auto [x0,y0,x1,y1,x2,y2] = std::tuple_cat(GetXY(p0), GetXY(p1), GetXY(p2));

    //sorting three coordinates, so that the topmost coordinate comes first.
    //ordering first by Y-coordinate and then by X-coordinate
    if( std::tie(y1,x1)<std::tie(y0,x0))
    {
        std::swap(x0,x1);
        std::swap(y0,y1);
        std::swap(p0,p1);
    }

    if( std::tie(y2,x2)<std::tie(y0,x0))
    {
        std::swap(x0,x2);
        std::swap(y0,y2);
        std::swap(p0,p2);
    }

    if( std::tie(y2,x2)<std::tie(y1,x1))
    {
        std::swap(x1,x2);
        std::swap(y1,y2);
        std::swap(p1,p2);
    }

    //If triangle has no area, return 
    if( y0==y2 ) return;

    bool shortSide = (y1-y0)*(x2-x0) < (x1-x0)*(y2-y0);
    //false: short side is in left side
    //true: short side is in right side

    std::pair<float,float> slopes[2];
    //slope of longest side
    slopes[!shortSide]=Makeslope(p0,p2,y2-y0);
    for ( auto y=y0, endy=y0; ; ++y)
    {
        if( y>=endy )
        {
            if( y>=y2 ) break;
            std::pair<float,float> arg;
            if( y<y1 )
            {
                endy=y1;
                arg = Makeslope(p0,p1,y1-y0);
            }
            else
            {
                endy=y2;
                arg = Makeslope(p1,p2,y2-y1);
            }
            slopes[shortSide]=arg;
        }
        DrawScanLine( y, slopes[0], slopes[1] , Plot );
    }
}