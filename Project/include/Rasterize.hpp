#pragma once
#include <tuple>
#include <vector>
#include <algorithm>
#include "glalib.hpp"


//rasterize triangle
template <typename T>
void RasterizeTriangle( T& p0, T& p1, T& p2, auto&& GetXY, auto&& Makeslope, auto&& DrawScanLine, auto&& Plot)
{   
    auto [x0,y0,x1,y1,x2,y2] = std::tuple_cat(GetXY(p0), GetXY(p1), GetXY(p2));

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
        DrawScanLine( y, slopes[0], slopes[1] , Plot);
    }
}