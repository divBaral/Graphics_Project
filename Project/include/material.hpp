#pragma once
#include <iostream>
//const material default_material = { {1, 1, 1}, {0.8, 0.8, 0.8}, {0.5, 0.5, 0.5}, 10 };

class material
{
public:
    float ns=10;
    float ka[3]={1,1,1};
    float ks[3]={0.8,0.8,0.8};
    float kd[3]={0.5,0.5,0.5};
    void print() {
        for ( int i =0; i<3; ++i ) {
            std::cout<<ka[i]<<' '<<std::endl;
            std::cout<<ks[i]<<' '<<std::endl;
        }
    }
};
