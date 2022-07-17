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
#include "objParser.hpp"
#include "glalib.hpp"

#define SCRWIDTH 800
#define SCRHEIGHT 600

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "SFML works!");

    window.setFramerateLimit(30);
    std::vector<std::vector<float>> verticesx;
    std::vector<std::vector<float>> normals;
    std::vector<std::vector<int>> faces;
    objLoader("/media/roshan/SSD/Projects/Graphics_Project/Project/res/House.obj", verticesx, normals, faces);

    Camera cam;
    Matrix4f Translate = af::Translate(Vector(25, 25, -25), Vector(0, 0, 0));

    Matrix4f viewport = cam.update({0.0f, 0, 100.f}, {0.0f, 0.0f, 0.0f});

    Matrix3f ToPixel = af2::PointsToPoints({-1, 1}, {1, 1}, {-1, -1},
                                           {0, 0}, {SCRWIDTH, 0}, {0, SCRHEIGHT});

    Renderer renderer(&window);

    // viewport = cam.update( {50,0,50}, {50,50,0} );
    sf::Clock clock;

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
                float xx = (tt + 100) * sin(f);
                float zz = (tt + 100) * cos(f);
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
            }
        }

        window.clear();

        // viewport=cam.update( {xx,25.0f+f,zz}, {50.0f,25.0f,0.0f} );

        // mapping points corresponding to the faces
        int i = 0;
        for (std::vector<int> face : faces)
        {
            // points p1, p2 and p3 are points of a face(triangle)
            Point p1 = {verticesx[face[0] - 1][0], verticesx[face[0] - 1][1], verticesx[face[0] - 1][2]};
            Point p2 = {verticesx[face[1] - 1][0], verticesx[face[1] - 1][1], verticesx[face[1] - 1][2]};
            Point p3 = {verticesx[face[2] - 1][0], verticesx[face[2] - 1][1], verticesx[face[2] - 1][2]};
            p1 = viewport * Translate * p1;
            p2 = viewport * Translate * p2;
            p3 = viewport * Translate * p3;

            p1.homogenize();
            p2.homogenize();
            p3.homogenize();

            Point2d q1 = {p1.x, p1.y};
            Point2d q2 = {p2.x, p2.y};
            Point2d q3 = {p3.x, p3.y};

            q1 = ToPixel * q1;
            q2 = ToPixel * q2;
            q3 = ToPixel * q3;

            renderer.DrawTriangle(q1.x, q1.y,
                                  q2.x, q2.y,
                                  q3.x, q3.y);
        }

        window.display();
    }

    return 0;
}