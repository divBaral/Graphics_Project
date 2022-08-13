#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <stdio.h>
#include "material.hpp"

void mtlLoader(std::string filePath, std::vector<std::string> &materials, std::map<std::string, material> &materialProperties)
{
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
        return;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string first_token;
        iss >> first_token;
        if (first_token == "#")
            continue;
        if (first_token == "newmtl")
        {
            std::string material_name;
            iss >> material_name;
            material m;
            while (true)
            {
                getline(file, line);
                std::istringstream pss(line);
                pss.str(line);
                std::string properties;
                pss >> properties;
                if (properties == "Ns")
                {
                    pss >> m.ns;
                }
                else if (properties == "Ka")
                {
                    pss >> m.ka[0] >> m.ka[1] >> m.ka[2];
                }
                else if (properties == "Ks")
                {
                    pss >> m.ks[0] >> m.ks[1] >> m.ks[2];
                }
                else if (properties == "Kd")
                {
                    pss >> m.kd[0] >> m.kd[1] >> m.kd[2];
                }
                // else if (properties == "illum" || properties == "Ke" || properties == "Ni" || properties == "d")
                //     break;
                else
                    break;
            }
            materialProperties[material_name] = m;
        }
    }
}