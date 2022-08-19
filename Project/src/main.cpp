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
#include "Triangle.hpp"
#include "glalib.hpp"
#include "DepthBuffer.hpp"
#include "kojuParser.hpp"
#define SCRWIDTH 800
#define SCRHEIGHT 600

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "SFML works!");
    window.setFramerateLimit(30);

    //load scene/mesh in vector
    std::vector<Triangle> scene;
    LoadObject(scene, "/home/baral/Downloads/graphicslearn/Project/res/models/house.mtl","/home/baral/Downloads/graphicslearn/Project/res/models/house.obj");

    //initiaze camera properties
    Camera cam;
    float RadiusOfSight = 30.f; 
    Point initialPos(0.f,0.f,RadiusOfSight);
    Point targetPos(0,0,0);
    Matrix4f viewspace = cam.update(initialPos, targetPos);
    Renderer renderer(&window);

    //transform vertex attributes ready for drawn
    std::vector<sf::Vertex> drawablescene;
    bool eventoccured = true;
    while (window.isOpen())
    {
        sf::Event event;

        static float f = 0;
        static float tt = 0.0f;
        static float yy = 0.f;
        float xx = (tt + RadiusOfSight) * sin(f);
        float zz = (tt + RadiusOfSight) * cos(f);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                eventoccured = true;
               
                if (zz == 0)
                {
                    zz = -4.f;
                }
               

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

        DepthBuffer depthbuffer(window.getSize().x, window.getSize().y, INFINITY);
        Point camepos(xx, yy, zz);
        
        //draw each triangle and add to drawablescene
        if ( eventoccured ) {
            drawablescene.resize(0);
            for ( const auto& t : scene ) {
                renderer.DrawTriangle(t, viewspace, depthbuffer, camepos, drawablescene);
            }
            eventoccured = false;
        }
        window.draw(&drawablescene[0], drawablescene.size(), sf::Points);
        window.display();
        
    }

    return 0;
}