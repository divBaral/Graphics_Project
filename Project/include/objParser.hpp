#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

void objLoader(std::string filePath, std::vector<std::vector<float>> vertices, std::vector<std::vector<float>> normals, std::vector<std::vector<float>> faces)
{
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
        return;

    while (file)
    {
        std::string line;
        getline(file, line);
        if (line[0] == 'v')
        {
            std::stringstream stream(line);
            std::vector<float> vertex;
            float a, b, c;
            char x;
            stream >> x >> a >> b >> c;
            vertex.push_back(a);
            vertex.push_back(b);
            vertex.push_back(c);
            vertices.push_back(vertex);
        }
        else if (line.substr(0, 2) == "vn")
        {
            std::stringstream stream(line);
            std::vector<float> normal;
            float a, b, c;
            std::string x;
            stream >> x >> a >> b >> c;
            normal.push_back(a);
            normal.push_back(b);
            normal.push_back(c);
            normals.push_back(normal);
        }
        else if (line[0] == 'f')
        {
            std::stringstream stream(line);
            std::vector<float> face;
            float a, b, c;
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
        }
    }
    for (std::vector<float> vertex : vertices)
    {
        for (float vert : vertex)
        {
            std::cout << vert << " ";
        }
        std::cout << std::endl;
    }
}