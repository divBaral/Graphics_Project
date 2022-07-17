#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <stdio.h>

void objLoader(std::string filePath, std::vector<std::vector<float>> &vertices, std::vector<std::vector<float>> &normals, std::vector<std::vector<int>> &faces, std::vector<std::string> &materials, std::map<std::string, std::vector<std::vector<int>>> &materialFaces)
{
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
        return;

    std::vector<std::vector<int>> tempFacesForMaterial;

    while (file)
    {
        std::string line;
        getline(file, line);
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream s(line.substr(2));
            std::vector<float> vertex;
            float a, b, c;
            s >> a >> b >> c;
            // std::cout << a << ":::" << b << ":::" << c << std::endl;
            vertex.push_back(a);
            vertex.push_back(b);
            vertex.push_back(c);
            vertices.push_back(vertex);
        }
        else if (line.substr(0, 3) == "vn ")
        {
            std::istringstream s(line.substr(2));
            std::vector<float> normal;
            float a, b, c;
            s >> a >> b >> c;
            normal.push_back(b);
            normal.push_back(a);
            normal.push_back(c);
            normals.push_back(normal);
        }
        else if (line.substr(0, 7) == "usemtl ")
        {
            std::istringstream s(line.substr(6));
            std::string material;
            s >> material;
            if (tempFacesForMaterial.size())
                materialFaces[materials.back()] = tempFacesForMaterial;
            tempFacesForMaterial.clear();
            materials.push_back(material);
        }
        else if (line.compare(0, 1, "f") == 0)
        {
            std::stringstream stream(line);
            std::vector<int> face;
            int a, b, c;
            std::string x;
            float ignore;
            std::string line1, line2, line3;
            stream >> x >> line1 >> line2 >> line3;
            std::stringstream stream1(line1), stream2(line2), stream3(line3);
            stream1 >> a;
            stream2 >> b;
            stream3 >> c;
            face.push_back(a);
            face.push_back(b);
            face.push_back(c);
            faces.push_back(face);
            tempFacesForMaterial.push_back(face);
        }
    }
    materialFaces[materials.back()] = tempFacesForMaterial;
}