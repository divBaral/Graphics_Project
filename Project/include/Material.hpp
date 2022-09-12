#pragma once
#include <iostream>
class Material
{
public:
    float ns=10;
    float ka[3]={1,1,1};
    float ks[3]={0.8,0.8,0.8};
    float kd[3]={0.5,0.5,0.5};
};
