#pragma once
#include <iostream>
class material
{
public:
    float ns;
    float ka[3];
    float ks[3];
    float kd[3];
    void print() {
        for ( int i =0; i<3; ++i ) {
            std::cout<<ka[i]<<' '<<std::endl;
            std::cout<<ks[i]<<' '<<std::endl;
        }
    }
};
