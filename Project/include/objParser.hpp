#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <stdio.h>
#include <Triangle.hpp>
#include "glalib.hpp"
#include "material.hpp"


void objLoader(std::string filePath, std::vector<std::string> &materials, std::map<std::string, std::vector<Triangle>> &materialTriangles)
{
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
        return;

    std::vector<Triangle> tempTriangles;
    std::string currentMaterial;
    std::vector<std::vector<float>> vertices;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<float>> textures;
    int i = 0;
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
            normal.push_back(a);
            normal.push_back(b);
            normal.push_back(c);
            normals.push_back(normal);
        }
        else if (line.substr(0, 3) == "vt ")
        {
            std::istringstream s(line.substr(2));
            std::vector<float> texture;
            float a, b;
            s >> a >> b;
            texture.push_back(a);
            texture.push_back(b);
            textures.push_back(texture);
        }
        else if (line.substr(0, 7) == "usemtl ")
        {
            std::istringstream s(line.substr(6));
            std::string material;
            s >> material;
            if (tempTriangles.size())
                materialTriangles[currentMaterial] = tempTriangles;
            tempTriangles.clear();
            if (!materialTriangles[material].size())
            {
                materials.push_back(material);
            }
            else
            {
                tempTriangles = materialTriangles[material];
            }
            currentMaterial = material;
        }
        else if (line.substr(0, 2) == "f ")
        {
            i++;
            std::stringstream stream(line);
            Triangle t;
            int a, b, c, normal1, normal2, normal3, texture1, texture2, texture3;
            char x;
            float ignore;
            std::string line1, line2, line3;
            char slash;
            stream >> x >> line1 >> line2 >> line3;
            std::stringstream stream1(line1), stream2(line2), stream3(line3);
            stream1 >> a >> slash >> texture1 >> slash >> normal1;
            stream2 >> b >> slash >> texture2 >> slash >> normal2;
            stream3 >> c >> slash >> texture3 >> slash >> normal3;
            // for (int i = 0; i < 3; i++)
            // {
            //     t.v0[i] = vertices[a - 1][i];
            //     t.v1[i] = vertices[b - 1][i];
            //     t.v2[i] = vertices[c - 1][i];

            //     t.n0[i] = normals[normal1 - 1][i];
            //     t.n1[i] = normals[normal2 - 1][i];
            //     t.n2[i] = normals[normal3 - 1][i];

            //     if (i == 2)
            //         break;

            //     t.t0[i] = textures[texture1 - 1][i];
            //     t.t1[i] = textures[texture2 - 1][i];
            //     t.t2[i] = textures[texture3 - 1][i];
            // }
            t.v0 = Point(vertices[a - 1][0], vertices[a - 1][1],vertices[a - 1][2]);
            t.v1 = Point(vertices[b - 1][0], vertices[b - 1][1],vertices[b- 1][2]);
            t.v2 = Point(vertices[c - 1][0], vertices[c - 1][1],vertices[c - 1][2]);

            t.n0 = Vector{normals[normal1 - 1][0], normals[normal1 - 1][0],normals[normal1 - 1][0]};
            t.n1 = Vector{normals[normal2 - 1][1], normals[normal2 - 1][1],normals[normal2 - 1][1]};
            t.n2 = Vector{normals[normal3 - 1][2], normals[normal3 - 1][2],normals[normal3 - 1][2]};

            t.tv0 = Vector{textures[texture1 - 1][0], textures[texture2 - 1][0],0};
            t.tv1 = Vector{textures[texture1 - 1][1], textures[texture2 - 1][1],0};
            t.tv2 = Vector{textures[texture1 - 1][2], textures[texture2 - 1][2],0};

            tempTriangles.push_back(t);
        }
        // else if(line.substr(0, 2) == "f ") {
		// 	std::string v1, v2, v3;
        //     std::stringstream iss;
		// 	iss >> v1 >> v2 >> v3;
		// 	std::string x1 = v1.substr(0, v1.find("/"));
		// 	std::string x2 = v2.substr(0, v2.find("/"));
		// 	std::string x3 = v3.substr(0, v3.find("/"));
		// 	Triangle t(vertices[stoi(x1)-1], vertices[stoi(x2)-1], vertices[stoi(x3)-1]);

		// 	if (std::count(v1.begin(), v1.end(), '/') == 2) {
		// 		int first_pos = v1.find("/");
		// 		int second_pos = v1.find("/", first_pos + 1);

		// 		std::string n0 = v1.substr(second_pos + 1, v1.size() - second_pos - 1);

		// 		first_pos = v2.find("/");
		// 		second_pos = v2.find("/", first_pos + 1);
		// 		std::string n1 = v2.substr(second_pos + 1, v2.size() - second_pos - 1);

		// 		first_pos = v3.find("/");
		// 		second_pos = v3.find("/", first_pos + 1);
		// 		std::string n2 = v3.substr(second_pos + 1, v3.size() - second_pos - 1);
		// 		t.n0 = normals[stoi(n0) - 1];
		// 		t.n1 = normals[stoi(n1) - 1];
		// 		t.n2 = normals[stoi(n2) - 1];
		// 		t.hasnormal = true;
		// 	}
		// 	//t.mtl = currentMaterial;
		// 	tempTriangles.push_back(t);
		// }
    }
    materialTriangles[currentMaterial] = tempTriangles;
}