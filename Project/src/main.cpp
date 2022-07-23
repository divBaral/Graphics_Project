#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <cmath>
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
    std::map<std::string, std::vector<std::vector<int>>> materialNormals; // faces mapped to normals
    std::vector<std::string> materials;
    std::map<std::string, std::vector<std::vector<int>>> materialFaces;
    std::map<std::string, sf::Image> images;
    objLoader("/media/roshan/SSD/Projects/Graphics_Project/Project/res/models/house.obj", verticesx, normals, materialNormals, materials, materialFaces);

    Camera cam;
    Matrix4f Translate = af::Translate(Vector(0, 0, 0), Vector(0, 0, 0));

    Matrix4f viewport = cam.update({0.0f, 0, 40.f}, {0.0f, 0.0f, 0.0f});

    Matrix3f ToPixel = af2::PointsToPoints({-1, 1}, {1, 1}, {-1, -1},
                                           {0, 0}, {SCRWIDTH, 0}, {0, SCRHEIGHT});

    loadTexture(materials, images);
    Renderer renderer(&window);

    // viewport = cam.update( {50,0,50}, {50,50,0} );
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
                static float yy = 0.f;

                if (event.key.code == sf::Keyboard::Left)
                {
                    f += 0.1;
                    viewport = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    f -= 0.1;
                    viewport = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::Up)
                {

                    yy += 1.f;
                    viewport = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::Down)
                {

                    yy -= 1.f;
                    viewport = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::W)
                {

                    tt -= 1.f;
                    viewport = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                else if (event.key.code == sf::Keyboard::S)
                {

                    tt += 1.f;
                    viewport = cam.update({xx, yy, zz}, {0.0f, 0.0f, 0.0f});
                }
                // clearing pixel information and z-buffer
                renderer.clear();
            }
        }

        window.clear();

        // viewport=cam.update( {xx,25.0f+f,zz}, {50.0f,25.0f,0.0f} );

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
                p1 = viewport * Translate * p1;
                p2 = viewport * Translate * p2;
                p3 = viewport * Translate * p3;

                // p1.homogenize();
                // p2.homogenize();
                // p3.homogenize();

                float depth = p1.z;

                Point2d q1 = {p1.x, p1.y};
                Point2d q2 = {p2.x, p2.y};
                Point2d q3 = {p3.x, p3.y};

                q1 = ToPixel * q1;
                q2 = ToPixel * q2;
                q3 = ToPixel * q3;

                renderer.DrawTriangle({q1.x, q1.y}, {q2.x, q2.y}, {q3.x, q3.y}, depth, images[material]);
                // renderer.DrawTriangle(q1,q2,q3, images[material]);
            }
        }
        window.draw(renderer.m_pixels, SCRHEIGHT * SCRWIDTH, sf::Points);
        window.display();
    }

    return 0;
}