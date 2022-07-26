#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <cmath>
#include <tuple>
#include <math.h>
// headers
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "objParser.hpp"
#include "glalib.hpp"
#include "Zbuffer.hpp"

#define SCRWIDTH 800
#define SCRHEIGHT 600

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "SFML works!");
    window.setFramerateLimit(30);
    sf::Texture texture;
    std::vector<std::vector<float>> verticesx;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<int>> faces;                                  
    std::map<std::string, std::vector<std::vector<int>>> materialNormals; 
    std::vector<std::string> materials;
    std::map<std::string, std::vector<std::vector<int>>> materialFaces;
    std::map<std::string, sf::Image> images;
    objLoader("D:/Graphics_Project/Graphics_Project/Project/res/models/house.obj", verticesx, normals, faces, materialNormals, materials, materialFaces);

    Camera cam;
    Matrix4f viewspace = cam.update({0.0f, 0.f, 40.f}, {0.0f, 0.0f, 0.0f});

    
    loadTexture(materials, images);
    Renderer renderer(&window) ;

    sf::Clock clock;

    for (std::string material : materials)
    {
        std::cout << material << std::endl;
    }
    while (window.isOpen())
    {
        sf::Event event;

        static float f = 0;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {

                static float tt = 0.0f;
                float xx = (tt + 40) * sin(f);
                float zz = (tt + 40) * cos(f);
                if( zz==0 ){ zz=-4.f; }
                static float yy = 0.f;

                if (event.key.code == sf::Keyboard::Left)
                {
                    f += 0.1;
                    viewspace = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    f -= 0.1;
                    viewspace = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::Up)
                {

                    yy += 1.f;
                    viewspace = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::Down)
                {

                    yy -= 1.f;
                    viewspace = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::W)
                {

                    tt -= 1.f;
                    viewspace = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::S)
                {

                    tt += 1.f;
                    viewspace = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
            }
        }

        window.clear();
        //clearing z-buffer
        renderer.clear();

        // frame begins
        // mapping points corresponding to the faces
        for (std::string material : materials)
        {
            for (std::vector<int> face : materialFaces[material])
            {
                // points p1, p2 and p3 are points of a face(triangle)
                Point p1 = {verticesx[face[0] - 1][0], verticesx[face[0] - 1][1], verticesx[face[0] - 1][2]};
                Point p2 = {verticesx[face[1] - 1][0], verticesx[face[1] - 1][1], verticesx[face[1] - 1][2]};
                Point p3 = {verticesx[face[2] - 1][0], verticesx[face[2] - 1][1], verticesx[face[2] - 1][2]};

                //transformation to the view space i.e. camera space
                p1 = viewspace * p1;
                p2 = viewspace * p2;
                p3 = viewspace * p3;

                p1.homogenize();
                p2.homogenize();
                p3.homogenize();

                if( p1.z<-1 || p2.z<-1 || p3.z<-1 ) continue;

                renderer.DrawTriangle(p1,p2,p3,images[material]);
            }
        }
        window.display();
    }

    return 0;
}